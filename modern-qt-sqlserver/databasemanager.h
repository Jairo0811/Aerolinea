#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QVector>

#include "ruta.h"
#include "aeronave.h"
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
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H