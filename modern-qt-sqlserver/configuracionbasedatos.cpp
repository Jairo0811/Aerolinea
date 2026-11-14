#include "configuracionbasedatos.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

namespace
{
constexpr auto NOMBRE_ARCHIVO = "database.ini";
constexpr qint64 TAMANO_MAXIMO_PLANTILLA = 64 * 1024;
}

QString ConfiguracionBaseDatos::rutaPortable(const QString& directorioAplicacion)
{
    if (directorioAplicacion.isEmpty()) {
        return {};
    }

    return QDir(directorioAplicacion).filePath(
        QStringLiteral("config/") + NOMBRE_ARCHIVO);
}

QString ConfiguracionBaseDatos::rutaUsuario()
{
    const QString directorio =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if (directorio.isEmpty()) {
        return {};
    }

    return QDir(directorio).filePath(NOMBRE_ARCHIVO);
}

bool ConfiguracionBaseDatos::crearDesdePlantilla(const QString& destino,
                                                 const QString& plantilla)
{
    if (destino.isEmpty() || QFileInfo::exists(destino)) {
        return false;
    }

    QFile origen(plantilla);
    if (!origen.open(QIODevice::ReadOnly) ||
        origen.size() > TAMANO_MAXIMO_PLANTILLA) {
        return false;
    }
    const QByteArray contenido = origen.readAll();

    if (!QDir().mkpath(QFileInfo(destino).absolutePath())) {
        return false;
    }

    // NewOnly garantiza que nunca se sobrescribe un archivo creado entre la
    // comprobación anterior y la apertura.
    QFile archivo(destino);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::NewOnly)) {
        return false;
    }

    if (archivo.write(contenido) != contenido.size() || !archivo.flush()) {
        archivo.close();
        archivo.remove();
        return false;
    }

    return true;
}

ConfiguracionResuelta ConfiguracionBaseDatos::resolver(
    const QString& directorioAplicacion,
    const QString& plantilla)
{
    ConfiguracionResuelta resultado;

    const QString portable = rutaPortable(directorioAplicacion);
    if (!portable.isEmpty() && QFileInfo(portable).isFile()) {
        resultado.origen = OrigenConfiguracion::Portable;
        resultado.ruta = portable;
        resultado.directorio = QFileInfo(portable).absolutePath();
        return resultado;
    }

    const QString usuario = rutaUsuario();
    if (usuario.isEmpty()) {
        return resultado;
    }
    resultado.directorio = QFileInfo(usuario).absolutePath();

    if (!QFileInfo::exists(usuario) &&
        !crearDesdePlantilla(usuario, plantilla)) {
        qWarning() << "No fue posible crear la configuración inicial del usuario.";
    }

    if (QFileInfo(usuario).isFile()) {
        resultado.origen = OrigenConfiguracion::Usuario;
        resultado.ruta = usuario;
    }

    return resultado;
}

QString ConfiguracionBaseDatos::describir(OrigenConfiguracion origen)
{
    switch (origen) {
    case OrigenConfiguracion::Portable:
        return QStringLiteral("portable");
    case OrigenConfiguracion::Usuario:
        return QStringLiteral("usuario");
    case OrigenConfiguracion::Predeterminada:
        return QStringLiteral("predeterminada");
    }

    return QStringLiteral("predeterminada");
}
