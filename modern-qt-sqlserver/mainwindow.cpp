#include "mainwindow.h"
#include "config.hpp"
#include "databasemanager.h"
#include "rutamanager.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QCoreApplication>
#include <QDialog>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
constexpr auto APP_LOGO_RESOURCE = ":/branding/aerolineacpp.png";
constexpr auto ITLA_LOGO_RESOURCE = ":/branding/itla.png";
constexpr auto APP_LOGO_FILE = "branding/aerolineacpp_logo.png";
constexpr auto ITLA_LOGO_FILE = "branding/itla_logo.png";

QPixmap loadBrandPixmap(const QString &resourcePath,
                        const QString &relativeFilePath)
{
    QPixmap pixmap(resourcePath);
    if (!pixmap.isNull())
        return pixmap;
    pixmap.load(QCoreApplication::applicationDirPath() + "/" +
                relativeFilePath);
    return pixmap;
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
        QLabel#lblOrigen, QLabel#lblDestino { color:#bae6fd; font-weight:600; }
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
        QPlainTextEdit { background:#071421; color:#dff6ff; border:1px solid #245b86; border-radius:12px; padding:14px; selection-background-color:#0369a1; font-family:Consolas,"Cascadia Code",monospace; font-size:13px; }
    )";
}

void setRouteState(QLabel *label, const QString &text, const QString &color,
                   const QString &background, const QString &border)
{
    label->setText(text);
    label->setStyleSheet(QString("color:%1;background:%2;border:1px solid "
                                 "%3;border-radius:10px;padding:6px "
                                 "12px;font-size:11px;font-weight:700;")
                             .arg(color, background, border));
}
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const QPixmap appLogo = loadBrandPixmap(APP_LOGO_RESOURCE, APP_LOGO_FILE);
    if (!appLogo.isNull()) {
        setWindowIcon(QIcon(appLogo));
        ui->lblBrandLogo->setPixmap(appLogo.scaled(170, 96, Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    }

    setStyleSheet(appStyleSheet());
    setRouteState(ui->lblEstadoRuta, "LISTO", "#67e8f9", "#082f49", "#0e7490");
    statusBar()->showMessage("Sistema listo");

    QAction *accionAcercaDe = new QAction("Acerca de", this);
    ui->menubar->addMenu("Ayuda")->addAction(accionAcercaDe);

    connect(accionAcercaDe, &QAction::triggered, this, [this]() {
        QDialog dialog(this);
        dialog.setWindowTitle("Acerca de AerolineaCPP");
        const QPixmap appLogoPixmap =
            loadBrandPixmap(APP_LOGO_RESOURCE, APP_LOGO_FILE);
        if (!appLogoPixmap.isNull())
            dialog.setWindowIcon(QIcon(appLogoPixmap));
        dialog.setMinimumSize(620, 640);
        dialog.resize(660, 680);
        dialog.setStyleSheet(R"(
            QDialog { background:#08111f; color:#e8f1ff; }
            QFrame { background:#0d1b2a; border:1px solid #1e3a5f; border-radius:16px; }
            QLabel { color:#e8f1ff; }
            QPushButton { background:#0284c7; color:white; border:none; border-radius:10px; padding:10px 18px; font-weight:700; }
            QPushButton:hover { background:#0ea5e9; }
        )");

        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(26, 24, 26, 24);
        layout->setSpacing(16);

        QFrame *brandingFrame = new QFrame(&dialog);
        QHBoxLayout *brandingLayout = new QHBoxLayout(brandingFrame);
        brandingLayout->setContentsMargins(18, 14, 18, 14);
        brandingLayout->setSpacing(18);
        QLabel *appLogoLabel = new QLabel(brandingFrame);
        if (!appLogoPixmap.isNull())
            appLogoLabel->setPixmap(appLogoPixmap.scaled(
                135, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        appLogoLabel->setAlignment(Qt::AlignCenter);
        QLabel *brandingText = new QLabel(brandingFrame);
        brandingText->setText(
            QString(
                "<span style='font-size:24px;font-weight:700;'>AerolineaCPP "
                "v%1</span><br><span "
                "style='color:#67e8f9;font-size:14px;'>Legacy Restoration "
                "Edition</span><br><span style='color:#94a3b8;'>C++17 • Qt 6 • "
                "SQL Server</span>")
                .arg(PROJECT_VERSION));
        brandingText->setTextFormat(Qt::RichText);
        brandingLayout->addWidget(appLogoLabel);
        brandingLayout->addWidget(brandingText, 1);

        QFrame *academicFrame = new QFrame(&dialog);
        QHBoxLayout *academicLayout = new QHBoxLayout(academicFrame);
        academicLayout->setContentsMargins(18, 16, 18, 16);
        academicLayout->setSpacing(20);
        QLabel *itlaLogoLabel = new QLabel(academicFrame);
        const QPixmap itlaPixmap =
            loadBrandPixmap(ITLA_LOGO_RESOURCE, ITLA_LOGO_FILE);
        if (!itlaPixmap.isNull()) {
            itlaLogoLabel->setPixmap(itlaPixmap.scaled(
                150, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            itlaLogoLabel->setMinimumSize(160, 100);
        } else {
            itlaLogoLabel->setText("ITLA");
            itlaLogoLabel->setStyleSheet(
                "font-size:28px;font-weight:700;color:#67e8f9;");
        }
        itlaLogoLabel->setAlignment(Qt::AlignCenter);
        QLabel *academicText = new QLabel(academicFrame);
        academicText->setWordWrap(true);
        academicText->setTextFormat(Qt::RichText);
        academicText->setText(
            "<b style='font-size:16px;'>Proyecto Original</b><br>Estructuras "
            "de Datos (SOF-012)<br>Instituto Tecnológico de Las Américas "
            "(ITLA)<br>Período académico 2018-C1");
        academicLayout->addWidget(itlaLogoLabel);
        academicLayout->addWidget(academicText, 1);

        QLabel *details = new QLabel(&dialog);
        details->setAlignment(Qt::AlignCenter);
        details->setWordWrap(true);
        details->setTextFormat(Qt::RichText);
        details->setText(
            "<p><b>Integrantes del proyecto original</b><br>Francis Jairo "
            "Matías Rosario — 2015-2984<br>Jorge de Jesús Torres Pérez — "
            "2016-3515<br>Sebastian Donastor Hernández — "
            "2016-3607</p><p><b>Profesor</b><br>Raydelto Hernández "
            "Perera</p><p><b>Modernización tecnológica</b><br>Migración de "
            "aplicación de consola a interfaz gráfica<br>C++17 • Qt 6 Widgets "
            "• Microsoft SQL Server • CMake<br>Junio 2026</p>");

        QPushButton *btnCerrar = new QPushButton("Cerrar", &dialog);
        connect(btnCerrar, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(brandingFrame);
        layout->addWidget(academicFrame);
        layout->addWidget(details, 1);
        layout->addWidget(btnCerrar);
        dialog.exec();
    });

    DatabaseManager db;
    if (db.conectar()) {
        ui->cmbOrigen->clear();
        ui->cmbDestino->clear();
        const QStringList destinos = db.obtenerDestinos();
        ui->cmbOrigen->addItems(destinos);
        ui->cmbDestino->addItems(destinos);
        ui->lblTotalDestinos->setText("Destinos registrados: " +
                                      QString::number(db.contarDestinos()));
        ui->lblTotalRutas->setText("Rutas registradas: " +
                                   QString::number(db.contarRutas()));
        ui->lblTotalAeronaves->setText("Aeronaves registradas: " +
                                       QString::number(db.contarAeronaves()));
        ui->lblTotalVuelos->setText("Vuelos registrados: " +
                                    QString::number(db.contarVuelos()));
        statusBar()->showMessage("Conectado a SQL Server • " +
                                 QString::number(destinos.size()) +
                                 " destinos disponibles");
    } else {
        ui->btnBuscar->setEnabled(false);
        ui->lblTotalDestinos->setText("Destinos registrados: 0");
        ui->lblTotalRutas->setText("Rutas registradas: 0");
        ui->lblTotalAeronaves->setText("Aeronaves registradas: 0");
        ui->lblTotalVuelos->setText("Vuelos registrados: 0");
        ui->txtResultado->setPlainText(
            "No fue posible conectar con SQL Server.\n\nVerifica la "
            "configuración de la base de datos.");
        setRouteState(ui->lblEstadoRuta, "SIN CONEXIÓN", "#fecaca", "#450a0a",
                      "#991b1b");
        statusBar()->showMessage("Sin conexión a SQL Server");
        QMessageBox::warning(
            this, "Error de conexión",
            "No fue posible conectar con SQL Server.\n\nVerifica que el "
            "servidor esté disponible y que la base de datos AerolineaDB haya "
            "sido creada.");
    }

    connect(ui->btnLimpiar, &QPushButton::clicked, this, [this]() {
        if (ui->cmbOrigen->count() > 0)
            ui->cmbOrigen->setCurrentIndex(0);
        if (ui->cmbDestino->count() > 1)
            ui->cmbDestino->setCurrentIndex(1);
        else if (ui->cmbDestino->count() > 0)
            ui->cmbDestino->setCurrentIndex(0);
        ui->txtResultado->clear();
        setRouteState(ui->lblEstadoRuta, "LISTO", "#67e8f9", "#082f49",
                      "#0e7490");
        statusBar()->showMessage("Búsqueda limpiada");
        ui->cmbOrigen->setFocus();
    });

    connect(ui->btnBuscar, &QPushButton::clicked, this, [this]() {
        const QString origen = ui->cmbOrigen->currentText().trimmed();
        const QString destino = ui->cmbDestino->currentText().trimmed();

        if (origen.isEmpty() || destino.isEmpty()) {
            ui->txtResultado->setPlainText(
                "Selecciona un origen y un destino.");
            setRouteState(ui->lblEstadoRuta, "REVISAR", "#fde68a", "#422006",
                          "#a16207");
            statusBar()->showMessage("Selecciona un origen y un destino");
            return;
        }
        if (origen.compare(destino, Qt::CaseInsensitive) == 0) {
            ui->txtResultado->setPlainText(
                "El origen y el destino son iguales.\n\nSelecciona dos "
                "destinos diferentes para buscar una ruta.");
            setRouteState(ui->lblEstadoRuta, "REVISAR", "#fde68a", "#422006",
                          "#a16207");
            statusBar()->showMessage("Origen y destino deben ser diferentes");
            return;
        }

        ui->btnBuscar->setEnabled(false);
        ui->btnBuscar->setText("Buscando...");
        setRouteState(ui->lblEstadoRuta, "BUSCANDO", "#bae6fd", "#0c4a6e",
                      "#0284c7");
        statusBar()->showMessage("Buscando ruta: " + origen + " → " + destino);
        QCoreApplication::processEvents();

        DatabaseManager db;
        if (!db.conectar()) {
            ui->txtResultado->setPlainText(
                "No fue posible conectar con SQL Server.");
            setRouteState(ui->lblEstadoRuta, "ERROR", "#fecaca", "#450a0a",
                          "#991b1b");
            statusBar()->showMessage("Error de conexión a SQL Server");
            ui->btnBuscar->setText("Buscar ruta");
            ui->btnBuscar->setEnabled(true);
            return;
        }

        RutaManager manager(db.obtenerRutas(), db.obtenerVuelos(),
                            db.obtenerAeronaves());
        const QString resultado = manager.buscarRuta(origen, destino);
        ui->txtResultado->setPlainText(resultado);

        const bool noEncontrada =
            resultado.contains("no existe", Qt::CaseInsensitive) ||
            resultado.contains("no se encontr", Qt::CaseInsensitive) ||
            resultado.contains("no hay", Qt::CaseInsensitive);
        if (noEncontrada) {
            setRouteState(ui->lblEstadoRuta, "NO DISPONIBLE", "#fde68a",
                          "#422006", "#a16207");
            statusBar()->showMessage("No se encontró una ruta disponible");
        } else {
            setRouteState(ui->lblEstadoRuta, "RUTA ENCONTRADA", "#bbf7d0",
                          "#052e16", "#15803d");
            statusBar()->showMessage("Ruta calculada: " + origen + " → " +
                                     destino);
        }
        ui->btnBuscar->setText("Buscar ruta");
        ui->btnBuscar->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
