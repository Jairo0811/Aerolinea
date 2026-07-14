#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rutamanager.h"
#include "databasemanager.h"

#include <QAction>
#include <QDialog>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/aerolineacpp_logo.png"));

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
            dialog.setWindowIcon(QIcon(":/aerolineacpp_logo.png"));
            dialog.setFixedSize(540, 560);

            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            layout->setContentsMargins(24, 20, 24, 20);
            layout->setSpacing(12);

            QLabel *logo = new QLabel(&dialog);
            QPixmap pixmap(":/itla_logo.png");

            if (!pixmap.isNull())
            {
                logo->setPixmap(
                    pixmap.scaled(
                        190,
                        110,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    )
                );
            }
            else
            {
                logo->setText("ITLA");
            }

            logo->setAlignment(Qt::AlignCenter);

            QLabel *texto = new QLabel(&dialog);
            texto->setAlignment(Qt::AlignCenter);
            texto->setWordWrap(true);
            texto->setTextFormat(Qt::RichText);
            texto->setText(
                "<h2>AerolineaCPP v1.0</h2>"
                "<p><i>Legacy Restoration Edition</i></p>"

                "<p><b>Proyecto Original</b><br>"
                "Desarrollado para la asignatura<br>"
                "Estructuras de Datos (SOF-012)<br>"
                "Instituto Tecnológico de Las Américas (ITLA)<br>"
                "Período académico 2018-C1</p>"

                "<p><b>Integrantes</b><br>"
                "Francis Jairo Matías Rosario - 2015-2984<br>"
                "Jorge de Jesús Torres Pérez - 2016-3515<br>"
                "Sebastian Donastor Hernández - 2016-3607</p>"

                "<p><b>Profesor</b><br>"
                "Raydelto Hernández Perera</p>"

                "<p><b>Modernización Tecnológica</b><br>"
                "Migración de aplicación de consola a interfaz gráfica<br>"
                "C++ • Qt 6 Widgets • SQL Server<br>"
                "CMake • Programación Orientada a Objetos<br>"
                "Junio 2026</p>"
            );

            QPushButton *btnCerrar = new QPushButton("Cerrar", &dialog);
            connect(
                btnCerrar,
                &QPushButton::clicked,
                &dialog,
                &QDialog::accept
            );

            layout->addWidget(logo);
            layout->addWidget(texto);
            layout->addStretch();
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
                return;
            }

            DatabaseManager db;

            if (!db.conectar())
            {
                ui->txtResultado->setPlainText(
                    "No fue posible conectar con SQL Server."
                );
                return;
            }

            RutaManager manager(
                db.obtenerRutas(),
                db.obtenerVuelos(),
                db.obtenerAeronaves()
            );

            const QString resultado = manager.buscarRuta(origen, destino);

            ui->txtResultado->setPlainText(resultado);
        }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}