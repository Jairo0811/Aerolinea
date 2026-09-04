#include "databasemanager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>

#include <cmath>

namespace
{
constexpr auto CONNECTION_NAME = "conexion_aerolinea";
constexpr auto DEFAULT_DRIVER = "ODBC Driver 17 for SQL Server";
constexpr auto DEFAULT_SERVER = "localhost\\SQLEXPRESS";
constexpr auto DEFAULT_DATABASE = "AerolineaDB";
QString quoteOdbcValue(QString value)
{
    value.replace("}", "}}");
    return "{" + value + "}";
}
}

DatabaseManager::DatabaseManager() = default;

QString DatabaseManager::obtenerRutaConfiguracion() const
{
    const QString applicationDirectory = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir(applicationDirectory).filePath("config/database.ini"),
        QDir(applicationDirectory).filePath("config/database.example.ini")
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
    bool encrypt = true;
    bool trustServerCertificate = false;
    QString user;
    QString password;

    if (!configurationPath.isEmpty()) {
        QSettings settings(configurationPath, QSettings::IniFormat);
        settings.beginGroup("database");

        driver = settings.value("driver", driver).toString().trimmed();
        server = settings.value("server", server).toString().trimmed();
        database = settings.value("database", database).toString().trimmed();
        trustedConnection = settings.value("trustedConnection", true).toBool();
        encrypt = settings.value("encrypt", true).toBool();
        trustServerCertificate =
            settings.value("trustServerCertificate", false).toBool();

        settings.endGroup();
    }

    if (!textoSeguro(driver, 128) || !textoSeguro(server, 255) ||
        !textoSeguro(database, 128)) {
        qWarning() << "La configuración de SQL Server contiene valores no válidos.";
        return {};
    }

    static const QRegularExpression databaseNamePattern(
        QStringLiteral("^[A-Za-z0-9_.-]{1,128}$"));
    static const QRegularExpression driverPattern(
        QStringLiteral("^ODBC Driver (17|18) for SQL Server$"));
    if (!databaseNamePattern.match(database).hasMatch() ||
        !driverPattern.match(driver).hasMatch()) {
        qWarning() << "El controlador o nombre de base de datos no es válido.";
        return {};
    }

    QString connectionString = QString("DRIVER=%1;SERVER=%2;DATABASE=%3;")
        .arg(quoteOdbcValue(driver), quoteOdbcValue(server),
             quoteOdbcValue(database));

    if (!encrypt) {
        qWarning() << "La conexión sin cifrado no está permitida.";
        return {};
    }
    connectionString += "Encrypt=yes;";
    connectionString += trustServerCertificate
        ? "TrustServerCertificate=yes;"
        : "TrustServerCertificate=no;";
    connectionString +=
        "Connection Timeout=10;ApplicationIntent=ReadOnly;APP={AerolineaCPP};";

    if (trustedConnection) {
        connectionString += "Trusted_Connection=yes;";
    } else {
        user = qEnvironmentVariable("AEROLINEA_DB_USER").trimmed();
        password = qEnvironmentVariable("AEROLINEA_DB_PASSWORD");
        if (!textoSeguro(user, 128) || !textoSeguro(password, 512)) {
            qWarning() << "Faltan credenciales seguras de SQL Server en el entorno.";
            return {};
        }

        connectionString += QString("UID=%1;PWD=%2;")
            .arg(quoteOdbcValue(user), quoteOdbcValue(password));
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

    const QString connectionString = construirCadenaConexion();
    if (connectionString.isEmpty()) {
        return false;
    }
    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qWarning() << "No fue posible conectar con SQL Server. Código del controlador:"
                   << db.lastError().nativeErrorCode();
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
    query.setForwardOnly(true);

    if (!query.exec(
            "SELECT TOP (1000) Nombre FROM Destinos ORDER BY Id")) {
        qWarning() << "Error consultando destinos:" << query.lastError().text();
        return lista;
    }

    while (query.next()) {
        const QString nombre = query.value("Nombre").toString().trimmed();
        if (textoSeguro(nombre, 100)) {
            lista << nombre;
        }
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
    query.setForwardOnly(true);

    const QString sql = R"(
        SELECT TOP (1000)
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
        const int id = query.value("Id").toInt();
        const QString origen = query.value("Origen").toString().trimmed();
        const QString destino = query.value("Destino").toString().trimmed();
        const int distancia = query.value("DistanciaKm").toInt();
        const int duracion = query.value("DuracionMinutos").toInt();
        if (id > 0 && textoSeguro(origen, 100) &&
            textoSeguro(destino, 100) && origen != destino &&
            distancia > 0 && distancia <= 25000 &&
            duracion > 0 && duracion <= 2880) {
            rutas.append(Ruta(id, origen, destino, distancia, duracion));
        }
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
    query.setForwardOnly(true);

    if (!query.exec(
            "SELECT TOP (1000) Id, Modelo, Capacidad, Fabricante "
            "FROM Aeronaves ORDER BY Id")) {
        qWarning() << "Error consultando aeronaves:" << query.lastError().text();
        return aeronaves;
    }

    while (query.next()) {
        const int id = query.value("Id").toInt();
        const QString modelo = query.value("Modelo").toString().trimmed();
        const int capacidad = query.value("Capacidad").toInt();
        const QString fabricante =
            query.value("Fabricante").toString().trimmed();
        if (id > 0 && textoSeguro(modelo, 100) &&
            textoSeguro(fabricante, 100) && capacidad > 0 &&
            capacidad <= 1000) {
            aeronaves.append(Aeronave(id, modelo, capacidad, fabricante));
        }
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
    query.setForwardOnly(true);

    if (!query.exec(
            "SELECT TOP (1000) Id, RutaId, AeronaveId, CodigoVuelo, Aerolinea, Precio "
            "FROM Vuelos ORDER BY Id")) {
        qWarning() << "Error consultando vuelos:" << query.lastError().text();
        return vuelos;
    }

    while (query.next()) {
        const int id = query.value("Id").toInt();
        const int rutaId = query.value("RutaId").toInt();
        const int aeronaveId = query.value("AeronaveId").toInt();
        const QString codigo =
            query.value("CodigoVuelo").toString().trimmed();
        const QString aerolinea =
            query.value("Aerolinea").toString().trimmed();
        const double precio = query.value("Precio").toDouble();
        if (id > 0 && rutaId > 0 && aeronaveId > 0 &&
            textoSeguro(codigo, 20) && textoSeguro(aerolinea, 100) &&
            std::isfinite(precio) && precio >= 0.0 && precio <= 1000000.0) {
            vuelos.append(Vuelo(id, rutaId, aeronaveId, codigo, aerolinea,
                                precio));
        }
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
    if (rutaId <= 0) {
        return {};
    }

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

    const int id = query.value("Id").toInt();
    const int storedRutaId = query.value("RutaId").toInt();
    const int aeronaveId = query.value("AeronaveId").toInt();
    const QString codigo = query.value("CodigoVuelo").toString().trimmed();
    const QString aerolinea = query.value("Aerolinea").toString().trimmed();
    const double precio = query.value("Precio").toDouble();
    if (id <= 0 || storedRutaId != rutaId || aeronaveId <= 0 ||
        !textoSeguro(codigo, 20) || !textoSeguro(aerolinea, 100) ||
        !std::isfinite(precio) || precio < 0.0 || precio > 1000000.0) {
        qWarning() << "SQL Server devolvió un vuelo fuera de los límites permitidos.";
        return {};
    }

    return Vuelo(id, storedRutaId, aeronaveId, codigo, aerolinea, precio);
}

bool DatabaseManager::textoSeguro(const QString& value,
                                  qsizetype maximumLength)
{
    if (value.isEmpty() || value.size() > maximumLength) {
        return false;
    }

    for (const QChar character : value) {
        if (character.isNull() || character.category() == QChar::Other_Control ||
            character.category() == QChar::Other_Format) {
            return false;
        }
    }

    return true;
}
