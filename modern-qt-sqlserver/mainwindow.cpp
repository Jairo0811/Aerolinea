#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rutamanager.h"
#include "databasemanager.h"

#include <QAction>
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
constexpr auto APP_LOGO = ":/branding/aerolineacpp.png";
constexpr auto ITLA_LOGO = ":/branding/itla.png";

QString appStyleSheet()
{
    return R"(
        QMainWindow, QWidget#centralwidget {
            background: #08111f;
            color: #e8f1ff;
        }

        QMenuBar {
            background: #08111f;
            color: #dbeafe;
            padding: 4px 8px;
        }

        QMenuBar::item:selected,
        QMenu::item:selected {
            background: #12345a;
        }

        QMenu {
            background: #0d1b2a;
            color: #e8f1ff;
            border: 1px solid #1f4d73;
        }

        QStatusBar {
            background: #08111f;
            color: #7dd3fc;
        }

        QFrame#headerFrame {
            background: #0d1b2a;
            border: 1px solid #164e63;
            border-radius: 18px;
        }

        QLabel#lblTitulo {
            color: #f8fafc;
            font-size: 28px;
            font-weight: 700;
        }

        QLabel#lblSubtitulo {
            color: #7dd3fc;
            font-size: 15px;
            font-weight: 600;
        }

        QLabel#lblTecnologias,
        QLabel#lblResultadoHint,
        QLabel#lblBuscarDescripcion,
        QLabel#lblFooter {
            color: #94a3b8;
        }

        QLabel#lblVersionBadge {
            color: #67e8f9;
            background: #082f49;
            border: 1px solid #0e7490;
            border-radius: 12px;
            padding: 8px 12px;
            font-weight: 600;
        }

        QFrame#cardDestinos,
        QFrame#cardRutas,
        QFrame#cardAeronaves,
        QFrame#cardVuelos {
            background: #0d1b2a;
            border: 1px solid #1e3a5f;
            border-radius: 15px;
            min-height: 88px;
        }

        QLabel#lblDestinosIcon,
        QLabel#lblRutasIcon,
        QLabel#lblAeronavesIcon,
        QLabel#lblVuelosIcon {
            font-size: 24px;
        }

        QLabel#lblTotalDestinos,
        QLabel#lblTotalRutas,
        QLabel#lblTotalAeronaves,
        QLabel#lblTotalVuelos {
            color: #e0f2fe;
            font-weight: 600;
            font-size: 13px;
        }

        QFrame#searchFrame,
        QFrame#resultFrame {
            background: #0d1b2a;
            border: 1px solid #1e3a5f;
            border-radius: 18px;
        }

        QLabel#lblBuscarTitulo,
        QLabel#lblResultado {
            color: #f8fafc;
            font-size: 20px;
            font-weight: 700;
        }

        QLabel#lblOrigen,
        QLabel#lblDestino {
            color: #bae6fd;
            font-weight: 600;
        }

        QComboBox {
            background: #102338;
            color: #f8fafc;
            border: 1px solid #245b86;
            border-radius: 10px;
            padding: 8px 12px;
            font-size: 14px;
        }

        QComboBox:hover,
        QComboBox:focus {
            border: 1px solid #38bdf8;
        }

        QComboBox QAbstractItemView {
            background: #102338;
            color: #f8fafc;
            selection-background-color: #0369a1;
            border: 1px solid #245b86;
        }

        QPushButton#btnBuscar {
            background: #0284c7;
            color: white;
            border: none;
            border-radius: 11px;
            font-size: 14px;
            font-weight: 700;
            padding: 10px 16px;
        }

        QPushButton#btnBuscar:hover {
            background: #0ea5e9;
        }

        QPushButton#btnBuscar:pressed {
            background: #0369a1;
        }

        QPushButton#btnBuscar:disabled {
            background: #334155;
            color: #94a3b8;
        }

        QPlainTextEdit {
            background: #071421;
            color: #dff6ff;
            border: 1px solid #245b86;
            border-radius: 12px;
            padding: 14px;
            selection-background-color: #0369a1;
            font-family: Consolas, "Cascadia Code", monospace;
            font-size: 13px;
        }
    )";
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(APP_LOGO));
    setStyleSheet(appStyleSheet());

    const QPixmap appLogo(APP_LOGO);
    if (!appLogo.isNull())
    {
        ui->lblBrandLogo->setPixmap(
            appLogo.scaled(
                145,
                84,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }

    statusBar()->showMessage("Sistema listo");

    QAction *accionAcercaDe = new QAction("Acerca de", this);
    ui->menubar->addMenu("Ayuda")->addAction(accionAcercaDe);

    connect(
        accionAcercaDe,
        &QAction::triggered,
        this,
        [this]()
        {
            QDialog dialog(this);
            dialog.setWindowTitle("Acerca de AerolineaCPP");
            dialog.setWindowIcon(QIcon(APP_LOGO));
            dialog.setMinimumSize(620, 640);
            dialog.resize(660, 680);

            dialog.setStyleSheet(R"(
                QDialog {
                    background: #08111f;
                    color: #e8f1ff;
                }

                QFrame {
                    background: #0d1b2a;
                    border: 1px solid #1e3a5f;
                    border-radius: 16px;
                }

                QLabel {
                    color: #e8f1ff;
                }

                QPushButton {
                    background: #0284c7;
                    color: white;
                    border: none;
                    border-radius: 10px;
                    padding: 10px 18px;
                    font-weight: 700;
                }

                QPushButton:hover {
                    background: #0ea5e9;
                }
            )");

            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            layout->setContentsMargins(26, 24, 26, 24);
            layout->setSpacing(16);

            QFrame *brandingFrame = new QFrame(&dialog);
            QHBoxLayout *brandingLayout = new QHBoxLayout(brandingFrame);
            brandingLayout->setContentsMargins(18, 14, 18, 14);
            brandingLayout->setSpacing(18);

            QLabel *appLogoLabel = new QLabel(brandingFrame);
            const QPixmap appLogoPixmap(APP_LOGO);
            if (!appLogoPixmap.isNull())
            {
                appLogoLabel->setPixmap(
                    appLogoPixmap.scaled(
                        135,
                        80,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    )
                );
            }
            appLogoLabel->setAlignment(Qt::AlignCenter);

            QLabel *brandingText = new QLabel(brandingFrame);
            brandingText->setText(
                "<span style='font-size:24px;font-weight:700;'>AerolineaCPP v1.0</span><br>"
                "<span style='color:#67e8f9;font-size:14px;'>Legacy Restoration Edition</span><br>"
                "<span style='color:#94a3b8;'>C++17 • Qt 6 • SQL Server</span>"
            );
            brandingText->setTextFormat(Qt::RichText);

            brandingLayout->addWidget(appLogoLabel);
            brandingLayout->addWidget(brandingText, 1);

            QFrame *academicFrame = new QFrame(&dialog);
            QHBoxLayout *academicLayout = new QHBoxLayout(academicFrame);
            academicLayout->setContentsMargins(18, 16, 18, 16);
            academicLayout->setSpacing(20);

            QLabel *itlaLogoLabel = new QLabel(academicFrame);
            const QPixmap itlaPixmap(ITLA_LOGO);

            if (!itlaPixmap.isNull())
            {
                itlaLogoLabel->setPixmap(
                    itlaPixmap.scaled(
                        150,
                        95,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    )
                );
            }
            else
            {
                itlaLogoLabel->setText("ITLA");
                itlaLogoLabel->setStyleSheet(
                    "font-size:28px;font-weight:700;color:#67e8f9;"
                );
            }

            itlaLogoLabel->setAlignment(Qt::AlignCenter);

            QLabel *academicText = new QLabel(academicFrame);
            academicText->setWordWrap(true);
            academicText->setTextFormat(Qt::RichText);
            academicText->setText(
                "<b style='font-size:16px;'>Proyecto Original</b><br>"
                "Estructuras de Datos (SOF-012)<br>"
                "Instituto Tecnológico de Las Américas (ITLA)<br>"
                "Período académico 2018-C1"
            );

            academicLayout->addWidget(itlaLogoLabel);
            academicLayout->addWidget(academicText, 1);

            QLabel *details = new QLabel(&dialog);
            details->setAlignment(Qt::AlignCenter);
            details->setWordWrap(true);
            details->setTextFormat(Qt::RichText);
            details->setText(
                "<p><b>Integrantes del proyecto original</b><br>"
                "Francis Jairo Matías Rosario — 2015-2984<br>"
                "Jorge de Jesús Torres Pérez — 2016-3515<br>"
                "Sebastian Donastor Hernández — 2016-3607</p>"

                "<p><b>Profesor</b><br>"
                "Raydelto Hernández Perera</p>"

                "<p><b>Modernización tecnológica</b><br>"
                "Migración de aplicación de consola a interfaz gráfica<br>"
                "C++17 • Qt 6 Widgets • Microsoft SQL Server • CMake<br>"
                "Junio 2026</p>"
            );

            QPushButton *btnCerrar = new QPushButton("Cerrar", &dialog);
            connect(
                btnCerrar,
                &QPushButton::clicked,
                &dialog,
                &QDialog::accept
            );

            layout->addWidget(brandingFrame);
            layout->addWidget(academicFrame);
            layout->addWidget(details, 1);
            layout->addWidget(btnCerrar);

            dialog.exec();
        }
    );

    DatabaseManager db;

    if (db.conectar())
    {
        ui->cmbOrigen->clear();
        ui->cmbDestino->clear();

        const QStringList destinos = db.obtenerDestinos();

        ui->cmbOrigen->addItems(destinos);
        ui->cmbDestino->addItems(destinos);

        ui->lblTotalDestinos->setText(
            "Destinos registrados: " +
            QString::number(db.contarDestinos())
        );

        ui->lblTotalRutas->setText(
            "Rutas registradas: " +
            QString::number(db.contarRutas())
        );

        ui->lblTotalAeronaves->setText(
            "Aeronaves registradas: " +
            QString::number(db.contarAeronaves())
        );

        ui->lblTotalVuelos->setText(
            "Vuelos registrados: " +
            QString::number(db.contarVuelos())
        );

        statusBar()->showMessage(
            "Conectado a SQL Server • " + QString::number(destinos.size()) +
            " destinos disponibles"
        );
    }
    else
    {
        ui->btnBuscar->setEnabled(false);

        ui->lblTotalDestinos->setText("Destinos registrados: 0");
        ui->lblTotalRutas->setText("Rutas registradas: 0");
        ui->lblTotalAeronaves->setText("Aeronaves registradas: 0");
        ui->lblTotalVuelos->setText("Vuelos registrados: 0");

        ui->txtResultado->setPlainText(
            "No fue posible conectar con SQL Server.\n\n"
            "Verifica la configuración de la base de datos."
        );

        statusBar()->showMessage("Sin conexión a SQL Server");

        QMessageBox::warning(
            this,
            "Error de conexión",
            "No fue posible conectar con SQL Server.\n\n"
            "Verifica que el servidor esté disponible y que la base de "
            "datos AerolineaDB haya sido creada."
        );
    }

    connect(
        ui->btnBuscar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            const QString origen = ui->cmbOrigen->currentText().trimmed();
            const QString destino = ui->cmbDestino->currentText().trimmed();

            if (origen.isEmpty() || destino.isEmpty())
            {
                ui->txtResultado->setPlainText(
                    "Selecciona un origen y un destino."
                );
                statusBar()->showMessage("Selecciona un origen y un destino");
                return;
            }

            if (origen.compare(destino, Qt::CaseInsensitive) == 0)
            {
                ui->txtResultado->setPlainText(
                    "El origen y el destino son iguales.\n\n"
                    "Selecciona dos destinos diferentes para buscar una ruta."
                );
                statusBar()->showMessage("Origen y destino deben ser diferentes");
                return;
            }

            DatabaseManager db;

            if (!db.conectar())
            {
                ui->txtResultado->setPlainText(
                    "No fue posible conectar con SQL Server."
                );
                statusBar()->showMessage("Error de conexión a SQL Server");
                return;
            }

            statusBar()->showMessage(
                "Buscando ruta: " + origen + " → " + destino
            );

            RutaManager manager(
                db.obtenerRutas(),
                db.obtenerVuelos(),
                db.obtenerAeronaves()
            );

            const QString resultado = manager.buscarRuta(origen, destino);

            ui->txtResultado->setPlainText(resultado);
            statusBar()->showMessage(
                "Ruta calculada: " + origen + " → " + destino
            );
        }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
