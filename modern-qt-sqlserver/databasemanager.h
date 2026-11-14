#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QVector>

#include "aeronave.h"
#include "configuracionbasedatos.h"
#include "ruta.h"
#include "vuelo.h"

class DatabaseManager
{
public:
    DatabaseManager();

    bool conectar();
    // Cierra la conexión compartida y vuelve a abrirla leyendo de nuevo la
    // configuración, de modo que se apliquen los cambios hechos por el usuario.
    bool reconectar();

    const ConfiguracionResuelta& configuracionActiva() const;

    QStringList obtenerDestinos();
    QVector<Ruta> obtenerRutas();
    QVector<Aeronave> obtenerAeronaves();
    QVector<Vuelo> obtenerVuelos();

    int contarDestinos();
    int contarRutas();
    int contarAeronaves();
    int contarVuelos();

private:
    QString construirCadenaConexion();
    static bool textoSeguro(const QString& value, qsizetype maximumLength);

    QSqlDatabase db;
    ConfiguracionResuelta configuracion;
};

#endif // DATABASEMANAGER_H
