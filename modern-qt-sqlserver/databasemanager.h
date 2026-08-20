#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QVector>

#include "aeronave.h"
#include "ruta.h"
#include "vuelo.h"

class DatabaseManager
{
public:
    DatabaseManager();

    bool conectar();

    QStringList obtenerDestinos();
    QVector<Ruta> obtenerRutas();
    QVector<Aeronave> obtenerAeronaves();
    QVector<Vuelo> obtenerVuelos();
    Vuelo obtenerVueloPorRutaId(int rutaId);

    int contarDestinos();
    int contarRutas();
    int contarAeronaves();
    int contarVuelos();

private:
    QString obtenerRutaConfiguracion() const;
    QString construirCadenaConexion() const;
    static bool textoSeguro(const QString& value, qsizetype maximumLength);

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
