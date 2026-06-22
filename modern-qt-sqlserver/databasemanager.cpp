#include "databasemanager.h"
#include "ruta.h"
#include "aeronave.h"
#include "vuelo.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::conectar()
{
    if (QSqlDatabase::contains("conexion_aerolinea")) {
        db = QSqlDatabase::database("conexion_aerolinea");
    } else {
        db = QSqlDatabase::addDatabase("QODBC", "conexion_aerolinea");
    }

    db.setDatabaseName(
        "DRIVER={ODBC Driver 17 for SQL Server};"
        "SERVER=MD-IT-08\\SQLEXPRESS;"
        "DATABASE=AerolineaDB;"
        "Trusted_Connection=yes;"
        );

    if (!db.open()) {
        qDebug() << "Error al conectar con SQL Server:" << db.lastError().text();
        return false;
    }

    qDebug() << "Conexión exitosa con SQL Server";
    return true;
}

QStringList DatabaseManager::obtenerDestinos()
{
    QStringList lista;

    if (!db.isOpen() && !conectar()) {
        return lista;
    }

    QSqlQuery query(db);

    if (!query.exec("SELECT Nombre FROM Destinos ORDER BY Id")) {
        qDebug() << "Error consultando destinos:" << query.lastError().text();
        return lista;
    }

    while (query.next()) {
        lista << query.value("Nombre").toString();
    }

    return lista;
}

QVector<Ruta> DatabaseManager::obtenerRutas()
{
    QVector<Ruta> rutas;

    if (!db.isOpen() && !conectar()) {
        return rutas;
    }

    QSqlQuery query(db);

    QString sql = R"(
    SELECT
        r.Id AS Id,
        o.Nombre AS Origen,
        d.Nombre AS Destino,
        r.DistanciaKm,
        r.DuracionMinutos
    FROM Rutas r
    INNER JOIN Destinos o ON r.OrigenId = o.Id
    INNER JOIN Destinos d ON r.DestinoId = d.Id
    ORDER BY r.Id
)";

    if (!query.exec(sql)) {
        qDebug() << "Error consultando rutas:" << query.lastError().text();
        return rutas;
    }

    while (query.next()) {
        rutas.append(Ruta(
            query.value("Id").toInt(),
            query.value("Origen").toString(),
            query.value("Destino").toString(),
            query.value("DistanciaKm").toInt(),
            query.value("DuracionMinutos").toInt()
            ));
    }

    return rutas;
}

QVector<Aeronave> DatabaseManager::obtenerAeronaves()
{
    QVector<Aeronave> aeronaves;

    if (!db.isOpen() && !conectar()) {
        return aeronaves;
    }

    QSqlQuery query(db);

    if (!query.exec(
            "SELECT Id, Modelo, Capacidad, Fabricante "
            "FROM Aeronaves "
            "ORDER BY Id"))
    {
        qDebug() << "Error consultando aeronaves:" << query.lastError().text();
        return aeronaves;
    }

    while (query.next()) {
        aeronaves.append(Aeronave(
            query.value("Id").toInt(),
            query.value("Modelo").toString(),
            query.value("Capacidad").toInt(),
            query.value("Fabricante").toString()
            ));
    }

    return aeronaves;
}

QVector<Vuelo> DatabaseManager::obtenerVuelos()
{
    QVector<Vuelo> vuelos;

    if (!db.isOpen() && !conectar()) {
        return vuelos;
    }

    QSqlQuery query(db);

    if (!query.exec(
            "SELECT Id, RutaId, AeronaveId, CodigoVuelo, Aerolinea, Precio "
            "FROM Vuelos "
            "ORDER BY Id"))
    {
        qDebug() << "Error consultando vuelos:" << query.lastError().text();
        return vuelos;
    }

    while (query.next()) {
        vuelos.append(Vuelo(
            query.value("Id").toInt(),
            query.value("RutaId").toInt(),
            query.value("AeronaveId").toInt(),
            query.value("CodigoVuelo").toString(),
            query.value("Aerolinea").toString(),
            query.value("Precio").toDouble()
            ));
    }

    return vuelos;
}

int DatabaseManager::contarDestinos()
{
    if (!db.isOpen() && !conectar()) return 0;

    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(*) FROM Destinos")) return 0;

    return query.next() ? query.value(0).toInt() : 0;
}

int DatabaseManager::contarRutas()
{
    if (!db.isOpen() && !conectar()) return 0;

    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(*) FROM Rutas")) return 0;

    return query.next() ? query.value(0).toInt() : 0;
}

int DatabaseManager::contarAeronaves()
{
    if (!db.isOpen() && !conectar()) return 0;

    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(*) FROM Aeronaves")) return 0;

    return query.next() ? query.value(0).toInt() : 0;
}

int DatabaseManager::contarVuelos()
{
    if (!db.isOpen() && !conectar()) return 0;

    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(*) FROM Vuelos")) return 0;

    return query.next() ? query.value(0).toInt() : 0;
}

Vuelo DatabaseManager::obtenerVueloPorRutaId(int rutaId)
{
    if (!db.isOpen() && !conectar()) {
        return Vuelo();
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT Id, RutaId, AeronaveId, CodigoVuelo, Aerolinea, Precio "
        "FROM Vuelos "
        "WHERE RutaId = ?"
        );

    query.addBindValue(rutaId);

    if (!query.exec()) {
        qDebug() << "Error consultando vuelo por ruta:" << query.lastError().text();
        return Vuelo();
    }

    if (query.next()) {
        return Vuelo(
            query.value("Id").toInt(),
            query.value("RutaId").toInt(),
            query.value("AeronaveId").toInt(),
            query.value("CodigoVuelo").toString(),
            query.value("Aerolinea").toString(),
            query.value("Precio").toDouble()
            );
    }

    return Vuelo();
}