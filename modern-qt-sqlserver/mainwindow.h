#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // Conecta (o reconecta leyendo de nuevo la configuración), llena los
    // combos y contadores y deja la interfaz en un estado coherente.
    bool cargarDatosDesdeBaseDeDatos();
    void mostrarEstadoSinConexion();
    void abrirCarpetaConfiguracion();
    void mostrarAcercaDe();
    void buscarRuta();
    void limpiarBusqueda();

    Ui::MainWindow *ui;
    DatabaseManager baseDatos;
    bool conectando = false;
};
#endif // MAINWINDOW_H
