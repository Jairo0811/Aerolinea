#include "databasemanager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>

namespace
{
constexpr auto CONNECTION_NAME = "conexion_aerolinea";
constexpr auto DEFAULT_DRIVER = "ODBC Driver 17 for SQL Server";
constexpr auto DEFAULT_SERVER = "localhost\\SQLEXPRESS";
constexpr auto DEFAULT_DATABASE = "AerolineaDB";
}

DatabaseManager::DatabaseManager() = default;

QString DatabaseManager::obtenerRutaConfiguracion() const
{
    const QString applicationDirectory = QCoreApplication::applicationDirPath();
    const QString currentDirectory = QDir::currentPath();

    const QStringList candidates = {
        QDir(applicationDirectory).filePath("config/database.ini"),
        QDir(currentDirectory).filePath("config/database.ini"),
        QDir(applicationDirectory).filePath("config/database.example.ini"),
        QDir(currentDirectory).filePath("config/database.example.ini")
    };

    for (const QString& candidate : candidates) {
        if (QFileInfo::exists(candidate)) {
            return candidate;
        }
    }

    return {};
}

QString DatabaseManager::construirCadenaConexion() const
{
    const QString configurationPath = obtenerRutaConfiguracion();

    QString driver = DEFAULT_DRIVER;
    QString server = DEFAULT_SERVER;
    QString database = DEFAULT_DATABASE;
    bool trustedConnection = true;
    QString user;
    QString password;

    if (!configurationPath.isEmpty()) {
        QSettings settings(configurationPath, QSettings::IniFormat);
        settings.beginGroup("database");

        driver = settings.value("driver", driver).toString().trimmed();
        server = settings.value("server", server).toString().trimmed();
        database = settings.value("database", database).toString().trimmed();
        trustedConnection = settings.value("trustedConnection", true).toBool();
        user = settings.value("user").toString().trimmed();
        password = settings.value("password").toString();

        settings.endGroup();
    }

    QString connectionString = QString(
        "DRIVER={%1};SERVER=%2;DATABASE=%3;")
        .arg(driver, server, database);

    if (trustedConnection) {
        connectionString += "Trusted_Connection=yes;";
    } else {
        connectionString += QString("UID=%1;PWD=%2;").arg(user, password);
    }

    return connectionString;
}

bool DatabaseManager::conectar()
{
    if (QSqlDatabase::contains(CONNECTION_NAME)) {
        db = QSqlDatabase::database(CONNECTION_NAME);
    } else {
        db = QSqlDatabase::addDatabase("QODBC", CONNECTION_NAME);
    }

    if (db.isOpen()) {
        return true;
    }

    db.setDatabaseName(construirCadenaConexion());

    if (!db.open()) {
        qWarning() << "Error al conectar con SQL Server:" << db.lastError().text();
        qWarning() << "Configuración utilizada:" << obtenerRutaConfiguracion();
        return false;
    }

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
        qWarning() << "Error consultando destinos:" << query.lastError().text();
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

    const QString sql = R"(
        SELECT
            r.Id,
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
        qWarning() << "Error consultando rutas:" << query.lastError().text();
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
            "FROM Aeronaves ORDER BY Id")) {
        qWarning() << "Error consultando aeronaves:" << query.lastError().text();
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
            "FROM Vuelos ORDER BY Id")) {
        qWarning() << "Error consultando vuelos:" << query.lastError().text();
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
    if (!db.isOpen() && !conectar()) {
        return 0;
    }

    QSqlQuery query(db);
    return query.exec("SELECT COUNT(*) FROM Destinos") && query.next()
        ? query.value(0).toInt()
        : 0;
}

int DatabaseManager::contarRutas()
{
    if (!db.isOpen() && !conectar()) {
        return 0;
    }

    QSqlQuery query(db);
    return query.exec("SELECT COUNT(*) FROM Rutas") && query.next()
        ? query.value(0).toInt()
        : 0;
}

int DatabaseManager::contarAeronaves()
{
    if (!db.isOpen() && !conectar()) {
        return 0;
    }

    QSqlQuery query(db);
    return query.exec("SELECT COUNT(*) FROM Aeronaves") && query.next()
        ? query.value(0).toInt()
        : 0;
}

int DatabaseManager::contarVuelos()
{
    if (!db.isOpen() && !conectar()) {
        return 0;
    }

    QSqlQuery query(db);
    return query.exec("SELECT COUNT(*) FROM Vuelos") && query.next()
        ? query.value(0).toInt()
        : 0;
}

Vuelo DatabaseManager::obtenerVueloPorRutaId(int rutaId)
{
    if (!db.isOpen() && !conectar()) {
        return {};
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT Id, RutaId, AeronaveId, CodigoVuelo, Aerolinea, Precio "
        "FROM Vuelos WHERE RutaId = ?"
    );
    query.addBindValue(rutaId);

    if (!query.exec()) {
        qWarning() << "Error consultando vuelo por ruta:" << query.lastError().text();
        return {};
    }

    if (!query.next()) {
        return {};
    }

    return Vuelo(
        query.value("Id").toInt(),
        query.value("RutaId").toInt(),
        query.value("AeronaveId").toInt(),
        query.value("CodigoVuelo").toString(),
        query.value("Aerolinea").toString(),
        query.value("Precio").toDouble()
    );
}
