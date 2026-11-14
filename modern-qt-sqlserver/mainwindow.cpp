#include "mainwindow.h"
#include "config.hpp"
#include "databasemanager.h"
#include "rutamanager.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QTime>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

namespace
{
constexpr auto APP_LOGO_RESOURCE = ":/branding/aerolineacpp.png";
constexpr auto ITLA_LOGO_RESOURCE = ":/branding/itla.png";

CriterioRuta criterioSeleccionado(int indice)
{
    switch (indice) {
    case 1:
        return CriterioRuta::MenorDistancia;
    case 2:
        return CriterioRuta::MenorDuracion;
    case 3:
        return CriterioRuta::MenorPrecio;
    default:
        return CriterioRuta::MenosEscalas;
    }
}

QString appStyleSheet()
{
    return R"(
        QMainWindow, QWidget#centralwidget { background:#08111f; color:#e8f1ff; }
        QMenuBar { background:#08111f; color:#dbeafe; padding:4px 8px; }
        QMenuBar::item:selected, QMenu::item:selected { background:#12345a; }
        QMenu { background:#0d1b2a; color:#e8f1ff; border:1px solid #1f4d73; }
        QStatusBar { background:#08111f; color:#7dd3fc; }
        QFrame#headerFrame { background:#0d1b2a; border:1px solid #164e63; border-radius:18px; }
        QLabel#lblTitulo { color:#f8fafc; font-size:28px; font-weight:700; }
        QLabel#lblSubtitulo { color:#7dd3fc; font-size:15px; font-weight:600; }
        QLabel#lblTecnologias, QLabel#lblResultadoHint, QLabel#lblBuscarDescripcion, QLabel#lblFooter { color:#94a3b8; }
        QLabel#lblVersionBadge { color:#67e8f9; background:#082f49; border:1px solid #0e7490; border-radius:12px; padding:8px 12px; font-weight:600; }
        QFrame#cardDestinos, QFrame#cardRutas, QFrame#cardAeronaves, QFrame#cardVuelos { background:#0d1b2a; border:1px solid #1e3a5f; border-radius:15px; min-height:88px; }
        QLabel#lblDestinosIcon, QLabel#lblRutasIcon, QLabel#lblAeronavesIcon, QLabel#lblVuelosIcon { color:#38bdf8; font-size:24px; font-weight:700; }
        QLabel#lblTotalDestinos, QLabel#lblTotalRutas, QLabel#lblTotalAeronaves, QLabel#lblTotalVuelos { color:#e0f2fe; font-weight:600; font-size:13px; }
        QFrame#searchFrame, QFrame#resultFrame { background:#0d1b2a; border:1px solid #1e3a5f; border-radius:18px; }
        QLabel#lblBuscarTitulo, QLabel#lblResultado { color:#f8fafc; font-size:20px; font-weight:700; }
        QLabel#lblOrigen, QLabel#lblDestino, QLabel#lblCriterio { color:#bae6fd; font-weight:600; }
        QLabel#lblEstadoRuta { color:#67e8f9; background:#082f49; border:1px solid #0e7490; border-radius:10px; padding:6px 12px; font-size:11px; font-weight:700; }
        QComboBox { background:#102338; color:#f8fafc; border:1px solid #245b86; border-radius:10px; padding:8px 12px; font-size:14px; }
        QComboBox:hover, QComboBox:focus { border:1px solid #38bdf8; }
        QComboBox QAbstractItemView { background:#102338; color:#f8fafc; selection-background-color:#0369a1; border:1px solid #245b86; }
        QPushButton#btnBuscar { background:#0284c7; color:white; border:none; border-radius:11px; font-size:14px; font-weight:700; padding:10px 16px; }
        QPushButton#btnBuscar:hover { background:#0ea5e9; }
        QPushButton#btnBuscar:pressed { background:#0369a1; }
        QPushButton#btnBuscar:disabled { background:#334155; color:#94a3b8; }
        QPushButton#btnLimpiar { background:#102338; color:#bae6fd; border:1px solid #245b86; border-radius:11px; font-size:14px; font-weight:600; padding:10px 14px; }
        QPushButton#btnLimpiar:hover { background:#173552; border-color:#38bdf8; color:white; }
        QPushButton#btnReconectar { background:#422006; color:#fde68a; border:1px solid #a16207; border-radius:11px; font-size:14px; font-weight:700; padding:10px 14px; }
        QPushButton#btnReconectar:hover { background:#5b2d0a; border-color:#facc15; color:white; }
        QScrollArea#scrollContenido, QWidget#scrollViewport { background:#08111f; border:none; }
        QScrollBar:vertical { background:#08111f; width:10px; margin:2px; }
        QScrollBar:horizontal { background:#08111f; height:10px; margin:2px; }
        QScrollBar::handle:vertical { background:#1e3a5f; border-radius:3px; min-height:32px; }
        QScrollBar::handle:horizontal { background:#1e3a5f; border-radius:3px; min-width:32px; }
        QScrollBar::handle:hover { background:#245b86; }
        QScrollBar::add-line, QScrollBar::sub-line, QScrollBar::add-page, QScrollBar::sub-page { background:none; border:none; width:0; height:0; }
        QPlainTextEdit { background:#071421; color:#dff6ff; border:1px solid #245b86; border-radius:12px; padding:14px; selection-background-color:#0369a1; font-family:Consolas,"Cascadia Code",monospace; font-size:13px; }
    )";
}

void setRouteState(QLabel* label,
                   const QString& text,
                   const QString& color,
                   const QString& background,
                   const QString& border)
{
    label->setText(text);
    label->setStyleSheet(
        QString("color:%1;background:%2;border:1px solid %3;border-radius:10px;"
                "padding:6px 12px;font-size:11px;font-weight:700;")
            .arg(color, background, border));
}

void setReadyState(QLabel* label)
{
    setRouteState(label, "LISTO", "#67e8f9", "#082f49", "#0e7490");
}

void setReviewState(QLabel* label)
{
    setRouteState(label, "REVISAR", "#fde68a", "#422006", "#a16207");
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // En pantallas pequeñas o con escalado alto (p. ej. 1366×768, o 1080p al
    // 150 %) el contenido se desplaza en lugar de quedar recortado.
    QWidget* contenido = takeCentralWidget();
    auto* desplazamiento = new QScrollArea(this);
    desplazamiento->setObjectName("scrollContenido");
    desplazamiento->viewport()->setObjectName("scrollViewport");
    desplazamiento->setFrameShape(QFrame::NoFrame);
    desplazamiento->setWidgetResizable(true);
    desplazamiento->setWidget(contenido);
    setCentralWidget(desplazamiento);

    if (const QScreen* pantalla = screen()) {
        const QSize disponible = pantalla->availableGeometry().size();
        // Margen aproximado para la barra de título del sistema.
        if (width() > disponible.width() || height() + 40 > disponible.height()) {
            setWindowState(windowState() | Qt::WindowMaximized);
        }
    }

    const QPixmap appLogo(APP_LOGO_RESOURCE);
    if (!appLogo.isNull()) {
        ui->lblBrandLogo->setPixmap(appLogo.scaled(
            170, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    setStyleSheet(appStyleSheet());
    ui->lblVersionBadge->setText(
        QString("v%1 • Final Portfolio Edition").arg(PROJECT_VERSION));
    ui->lblFooter->setText(
        QString("AerolineaCPP v%1 • Proyecto original ITLA 2018-C1 • Final Portfolio Edition 2026")
            .arg(PROJECT_VERSION));
    ui->btnReconectar->setVisible(false);

    QMenu* menuBaseDatos = ui->menubar->addMenu("Base de datos");
    QAction* accionReconectar = menuBaseDatos->addAction("Reintentar conexión");
    accionReconectar->setShortcut(QKeySequence::Refresh);
    QAction* accionCarpeta =
        menuBaseDatos->addAction("Abrir carpeta de configuración");

    QAction* accionAcercaDe = ui->menubar->addMenu("Ayuda")->addAction("Acerca de");

    connect(accionReconectar, &QAction::triggered, this,
            [this]() { cargarDatosDesdeBaseDeDatos(); });
    connect(ui->btnReconectar, &QPushButton::clicked, this,
            [this]() { cargarDatosDesdeBaseDeDatos(); });
    connect(accionCarpeta, &QAction::triggered, this,
            &MainWindow::abrirCarpetaConfiguracion);
    connect(accionAcercaDe, &QAction::triggered, this, &MainWindow::mostrarAcercaDe);
    connect(ui->btnLimpiar, &QPushButton::clicked, this, &MainWindow::limpiarBusqueda);
    connect(ui->btnBuscar, &QPushButton::clicked, this, &MainWindow::buscarRuta);

    // La carga inicial se difiere hasta que la ventana es visible para que el
    // aviso de error aparezca sobre ella y no como un diálogo huérfano.
    setReadyState(ui->lblEstadoRuta);
    ui->btnBuscar->setEnabled(false);
    QTimer::singleShot(0, this, [this]() {
        if (!cargarDatosDesdeBaseDeDatos()) {
            QMessageBox::warning(
                this,
                "Error de conexión",
                "No fue posible conectar con SQL Server.\n\n"
                "Revisa el servidor, TLS, las credenciales y AerolineaDB en la "
                "configuración indicada en el panel de resultados. Después usa "
                "«Reintentar conexión».");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::cargarDatosDesdeBaseDeDatos()
{
    // F5 y el botón comparten esta ruta; evita reentradas mientras se conecta.
    if (conectando) {
        return false;
    }
    conectando = true;

    ui->btnBuscar->setEnabled(false);
    ui->btnReconectar->setEnabled(false);
    setRouteState(ui->lblEstadoRuta, "CONECTANDO", "#bae6fd", "#0c4a6e", "#0284c7");
    statusBar()->showMessage("Conectando con SQL Server...");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QCoreApplication::processEvents();

    const bool conectado = baseDatos.reconectar();

    QApplication::restoreOverrideCursor();
    ui->btnReconectar->setEnabled(true);
    conectando = false;

    if (!conectado) {
        mostrarEstadoSinConexion();
        return false;
    }

    const QStringList destinos = baseDatos.obtenerDestinos();
    ui->cmbOrigen->clear();
    ui->cmbDestino->clear();
    ui->cmbOrigen->addItems(destinos);
    ui->cmbDestino->addItems(destinos);
    // Igual que «Limpiar»: origen y destino distintos desde el inicio.
    if (ui->cmbDestino->count() > 1) {
        ui->cmbDestino->setCurrentIndex(1);
    }
    ui->lblTotalDestinos->setText(
        "Destinos registrados: " + QString::number(baseDatos.contarDestinos()));
    ui->lblTotalRutas->setText(
        "Rutas registradas: " + QString::number(baseDatos.contarRutas()));
    ui->lblTotalAeronaves->setText(
        "Aeronaves registradas: " + QString::number(baseDatos.contarAeronaves()));
    ui->lblTotalVuelos->setText(
        "Vuelos registrados: " + QString::number(baseDatos.contarVuelos()));

    ui->btnReconectar->setVisible(false);
    ui->btnBuscar->setEnabled(true);
    ui->txtResultado->clear();
    setReadyState(ui->lblEstadoRuta);
    statusBar()->showMessage(
        "Conectado a SQL Server • " + QString::number(destinos.size()) +
        " destinos disponibles • configuración " +
        ConfiguracionBaseDatos::describir(baseDatos.configuracionActiva().origen));
    return true;
}

void MainWindow::mostrarEstadoSinConexion()
{
    ui->btnBuscar->setEnabled(false);
    ui->btnReconectar->setVisible(true);
    ui->cmbOrigen->clear();
    ui->cmbDestino->clear();
    ui->lblTotalDestinos->setText("Destinos registrados: 0");
    ui->lblTotalRutas->setText("Rutas registradas: 0");
    ui->lblTotalAeronaves->setText("Aeronaves registradas: 0");
    ui->lblTotalVuelos->setText("Vuelos registrados: 0");

    const ConfiguracionResuelta& configuracion = baseDatos.configuracionActiva();
    const QString archivo = configuracion.ruta.isEmpty()
        ? QString("valores predeterminados (no se encontró database.ini)")
        : QDir::toNativeSeparators(configuracion.ruta);

    ui->txtResultado->setPlainText(
        "No fue posible conectar con SQL Server.\n\n"
        "Configuración en uso (" +
        ConfiguracionBaseDatos::describir(configuracion.origen) + "):\n" +
        archivo +
        "\n\nPasos sugeridos:\n"
        "1. Menú Base de datos → Abrir carpeta de configuración.\n"
        "2. Ajusta servidor, base de datos y opciones TLS en database.ini.\n"
        "3. Para autenticación SQL define AEROLINEA_DB_USER y AEROLINEA_DB_PASSWORD.\n"
        "4. Pulsa «Reintentar conexión» (F5).");
    setRouteState(ui->lblEstadoRuta, "SIN CONEXIÓN", "#fecaca", "#450a0a", "#991b1b");
    statusBar()->showMessage(
        "Sin conexión a SQL Server • último intento " +
        QTime::currentTime().toString("HH:mm:ss"));
}

void MainWindow::abrirCarpetaConfiguracion()
{
    ConfiguracionResuelta configuracion = baseDatos.configuracionActiva();
    if (configuracion.directorio.isEmpty()) {
        configuracion = ConfiguracionBaseDatos::resolver(
            QCoreApplication::applicationDirPath(),
            ":/config/database.example.ini");
    }

    if (configuracion.directorio.isEmpty() ||
        !QDir().mkpath(configuracion.directorio) ||
        !QDesktopServices::openUrl(QUrl::fromLocalFile(configuracion.directorio))) {
        QMessageBox::information(
            this,
            "Carpeta de configuración",
            "No fue posible abrir la carpeta de configuración automáticamente.\n\n" +
                QDir::toNativeSeparators(configuracion.directorio));
    }
}

void MainWindow::limpiarBusqueda()
{
    if (ui->cmbOrigen->count() > 0) {
        ui->cmbOrigen->setCurrentIndex(0);
    }
    if (ui->cmbDestino->count() > 1) {
        ui->cmbDestino->setCurrentIndex(1);
    } else if (ui->cmbDestino->count() > 0) {
        ui->cmbDestino->setCurrentIndex(0);
    }
    ui->cmbCriterio->setCurrentIndex(0);
    ui->txtResultado->clear();
    setReadyState(ui->lblEstadoRuta);
    statusBar()->showMessage("Búsqueda limpiada");
    ui->cmbOrigen->setFocus();
}

void MainWindow::buscarRuta()
{
    const QString origen = ui->cmbOrigen->currentText().trimmed();
    const QString destino = ui->cmbDestino->currentText().trimmed();
    const CriterioRuta criterio = criterioSeleccionado(ui->cmbCriterio->currentIndex());

    if (origen.isEmpty() || destino.isEmpty()) {
        ui->txtResultado->setPlainText("Selecciona un origen y un destino.");
        setReviewState(ui->lblEstadoRuta);
        statusBar()->showMessage("Selecciona un origen y un destino");
        return;
    }

    if (origen.compare(destino, Qt::CaseInsensitive) == 0) {
        ui->txtResultado->setPlainText(
            "El origen y el destino son iguales.\n\nSelecciona dos destinos diferentes.");
        setReviewState(ui->lblEstadoRuta);
        statusBar()->showMessage("Origen y destino deben ser diferentes");
        return;
    }

    ui->btnBuscar->setEnabled(false);
    ui->btnBuscar->setText("Calculando...");
    setRouteState(ui->lblEstadoRuta, "CALCULANDO", "#bae6fd", "#0c4a6e", "#0284c7");
    statusBar()->showMessage(
        "Optimizando " + origen + " → " + destino + " • " + RutaManager::nombreCriterio(criterio));
    QCoreApplication::processEvents();

    if (!baseDatos.conectar()) {
        ui->btnBuscar->setText("Calcular ruta");
        mostrarEstadoSinConexion();
        setRouteState(ui->lblEstadoRuta, "ERROR", "#fecaca", "#450a0a", "#991b1b");
        return;
    }

    const QVector<Ruta> rutas = baseDatos.obtenerRutas();
    if (rutas.isEmpty()) {
        // La conexión sigue abierta pero la lectura falló (p. ej. el servidor
        // se reinició): se ofrece reconectar en lugar de un resultado engañoso.
        ui->btnBuscar->setText("Calcular ruta");
        mostrarEstadoSinConexion();
        ui->txtResultado->setPlainText(
            "No fue posible leer las rutas desde SQL Server.\n\n"
            "Pulsa «Reintentar conexión» (F5) para volver a conectar.");
        setRouteState(ui->lblEstadoRuta, "ERROR", "#fecaca", "#450a0a", "#991b1b");
        return;
    }

    const RutaManager manager(
        rutas, baseDatos.obtenerVuelos(), baseDatos.obtenerAeronaves());
    const bool encontrada = !manager.calcularRuta(origen, destino, criterio).isEmpty();
    ui->txtResultado->setPlainText(manager.buscarRuta(origen, destino, criterio));

    if (encontrada) {
        setRouteState(ui->lblEstadoRuta, "RUTA ÓPTIMA", "#bbf7d0", "#052e16", "#15803d");
        statusBar()->showMessage(
            "Ruta calculada • " + RutaManager::nombreCriterio(criterio));
    } else {
        setRouteState(ui->lblEstadoRuta, "NO DISPONIBLE", "#fde68a", "#422006", "#a16207");
        statusBar()->showMessage("No se encontró una ruta compatible con el criterio");
    }

    ui->btnBuscar->setText("Calcular ruta");
    ui->btnBuscar->setEnabled(true);
}

void MainWindow::mostrarAcercaDe()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Acerca de");
    dialog.setMinimumSize(620, 620);
    dialog.resize(660, 650);
    dialog.setStyleSheet(R"(
        QDialog { background:#08111f; color:#e8f1ff; }
        /* Solo las tarjetas: QLabel hereda de QFrame y no debe llevar borde. */
        QFrame#aboutCard { background:#0d1b2a; border:1px solid #1e3a5f; border-radius:16px; }
        QLabel { color:#e8f1ff; background:transparent; }
        QPushButton { background:#0284c7; color:white; border:none; border-radius:10px; padding:10px 18px; font-weight:700; }
        QPushButton:hover { background:#0ea5e9; }
    )");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(16);

    QFrame* brandingFrame = new QFrame(&dialog);
    brandingFrame->setObjectName("aboutCard");
    QHBoxLayout* brandingLayout = new QHBoxLayout(brandingFrame);
    brandingLayout->setContentsMargins(18, 14, 18, 14);
    brandingLayout->setSpacing(18);

    QLabel* appLogoLabel = new QLabel(brandingFrame);
    const QPixmap appLogoPixmap = QPixmap(APP_LOGO_RESOURCE);
    if (!appLogoPixmap.isNull()) {
        appLogoLabel->setPixmap(appLogoPixmap.scaled(
            135, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    appLogoLabel->setAlignment(Qt::AlignCenter);

    QLabel* brandingText = new QLabel(brandingFrame);
    brandingText->setTextFormat(Qt::RichText);
    brandingText->setText(
        QString("<span style='font-size:24px;font-weight:700;'>AerolineaCPP v%1</span>"
                "<br><span style='color:#67e8f9;font-size:14px;'>Final Portfolio Edition</span>"
                "<br><span style='color:#94a3b8;'>C++17 • Qt 6 • SQL Server • Dijkstra</span>")
            .arg(PROJECT_VERSION));
    brandingLayout->addWidget(appLogoLabel);
    brandingLayout->addWidget(brandingText, 1);

    QFrame* academicFrame = new QFrame(&dialog);
    academicFrame->setObjectName("aboutCard");
    QHBoxLayout* academicLayout = new QHBoxLayout(academicFrame);
    academicLayout->setContentsMargins(18, 16, 18, 16);
    academicLayout->setSpacing(20);

    QLabel* itlaLogoLabel = new QLabel(academicFrame);
    const QPixmap itlaPixmap = QPixmap(ITLA_LOGO_RESOURCE);
    if (!itlaPixmap.isNull()) {
        itlaLogoLabel->setPixmap(itlaPixmap.scaled(
            150, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        itlaLogoLabel->setMinimumSize(160, 100);
        // Placa clara para que el logo institucional (azul oscuro) tenga
        // contraste sobre el tema oscuro sin alterar sus colores.
        itlaLogoLabel->setStyleSheet(
            "background:#f8fafc;border-radius:12px;padding:4px;");
    } else {
        itlaLogoLabel->setText("ITLA");
        itlaLogoLabel->setStyleSheet(
            "font-size:28px;font-weight:700;color:#67e8f9;");
    }
    itlaLogoLabel->setAlignment(Qt::AlignCenter);

    QLabel* academicText = new QLabel(academicFrame);
    academicText->setWordWrap(true);
    academicText->setTextFormat(Qt::RichText);
    academicText->setText(
        "<b style='font-size:16px;'>Proyecto Original</b><br>"
        "Estructuras de Datos (SOF-012)<br>"
        "Instituto Tecnológico de Las Américas (ITLA)<br>Período 2018-C1");
    academicLayout->addWidget(itlaLogoLabel);
    academicLayout->addWidget(academicText, 1);

    QLabel* details = new QLabel(&dialog);
    details->setObjectName("aboutCard");
    details->setContentsMargins(16, 12, 16, 12);
    details->setAlignment(Qt::AlignCenter);
    details->setWordWrap(true);
    details->setTextFormat(Qt::RichText);
    details->setText(
        "<p><b>Integrantes del proyecto original</b><br>"
        "Francis Jairo Matías Rosario — 2015-2984<br>"
        "Jorge de Jesús Torres Pérez — 2016-3515<br>"
        "Sebastian Donastor Hernández — 2016-3607</p>"
        "<p><b>Profesor</b><br>Raydelto Hernández Perera</p>"
        "<p><b>Evolución 2026</b><br>Qt 6 + SQL Server + seguridad reforzada + "
        "optimización multicriterio de rutas + pruebas automatizadas.</p>");

    QPushButton* btnCerrar = new QPushButton("Cerrar", &dialog);
    connect(btnCerrar, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(brandingFrame);
    layout->addWidget(academicFrame);
    layout->addWidget(details, 1);
    layout->addWidget(btnCerrar);
    dialog.exec();
}
