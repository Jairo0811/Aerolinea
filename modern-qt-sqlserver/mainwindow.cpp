#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rutamanager.h"
#include "databasemanager.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QAction>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *accionAcercaDe = new QAction("Acerca de", this);
    ui->menubar->addMenu("Ayuda")->addAction(accionAcercaDe);

    connect(accionAcercaDe, &QAction::triggered, this, [=]()
            {
        QDialog dialog(this);
        dialog.setWindowTitle("Acerca de AerolineaCPP");
        dialog.setFixedSize(520, 520);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *logo = new QLabel(&dialog);
QPixmap pixmap("C:/Users/fmatias/source/repos/AerolineaCPP/itla_logo.png");
        qDebug() << "Logo cargado:" << !pixmap.isNull();
        qDebug() << "Ancho:" << pixmap.width();
        qDebug() << "Alto:" << pixmap.height();
        logo->setPixmap(pixmap.scaled(180, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logo->setAlignment(Qt::AlignCenter);

        QLabel *texto = new QLabel(&dialog);
        texto->setAlignment(Qt::AlignCenter);
        texto->setWordWrap(true);
        texto->setText(

            "<h2>AerolineaCPP v1.0</h2>"

            "<p><b>Proyecto Original</b><br>"
            "Desarrollado para la asignatura<br>"
            "Estructuras de Datos (SOF-012)<br>"
            "Instituto Tecnológico de Las Américas (ITLA)<br>"
            "Abril 2018</p>"

            "<p><b>Integrantes</b><br>"
            "Francis Jairo Matías Rosario - 2015-2984<br>"
            "Jorge de Jesús Torres Pérez - 2016-3515<br>"
            "Sebastian Donastor Hernández - 2016-3607</p>"
            "<p><b>Profesor</b><br>"
            "Raydelto Hernández Perera</p>"

            "<p><b>Modernización Tecnológica</b><br>"
            "Migración de aplicación de consola a interfaz gráfica<br>"
            "Qt 6 + SQL Server<br>"
            "Junio 2026</p>"
            );

        QPushButton *btnCerrar = new QPushButton("Cerrar", &dialog);
        connect(btnCerrar, &QPushButton::clicked, &dialog, &QDialog::accept);

        layout->addWidget(logo);
        layout->addWidget(texto);
        layout->addWidget(btnCerrar);

        dialog.exec(); });

    DatabaseManager db;

    if (db.conectar())
    {
        ui->cmbOrigen->addItems(db.obtenerDestinos());
        ui->cmbDestino->addItems(db.obtenerDestinos());

        ui->lblTotalDestinos->setText(
            "Destinos registrados: " + QString::number(db.contarDestinos()));

        ui->lblTotalRutas->setText(
            "Rutas registradas: " + QString::number(db.contarRutas()));

        ui->lblTotalAeronaves->setText(
            "Aeronaves registradas: " + QString::number(db.contarAeronaves()));

        ui->lblTotalVuelos->setText(
            "Vuelos registrados: " + QString::number(db.contarVuelos()));
    }

    connect(ui->btnBuscar, &QPushButton::clicked, this, [=]()
            {
        DatabaseManager db;
        db.conectar();

        RutaManager manager(
            db.obtenerRutas(),
            db.obtenerVuelos(),
            db.obtenerAeronaves()
            );

        QString origen = ui->cmbOrigen->currentText();
        QString destino = ui->cmbDestino->currentText();

        QString resultado = manager.buscarRuta(origen, destino);

        ui->txtResultado->setPlainText(resultado); });
}

MainWindow::~MainWindow()
{
    delete ui;
}