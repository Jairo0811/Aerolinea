#ifndef CONFIGURACIONBASEDATOS_H
#define CONFIGURACIONBASEDATOS_H

#include <QString>

// Resolución del archivo database.ini.
//
// Orden de precedencia:
//   1. Portable: <directorio del ejecutable>/config/database.ini, si existe.
//   2. Usuario:  QStandardPaths::AppConfigLocation/database.ini. Si no existe,
//                se crea una única vez a partir de la plantilla; nunca se
//                sobrescribe un archivo existente.
//   3. Predeterminada: valores seguros internos de DatabaseManager.
//
// database.example.ini es solo una plantilla y nunca se lee como
// configuración del usuario.
enum class OrigenConfiguracion
{
    Portable,
    Usuario,
    Predeterminada
};

struct ConfiguracionResuelta
{
    OrigenConfiguracion origen = OrigenConfiguracion::Predeterminada;
    // Archivo INI en uso. Vacío cuando se usan los valores predeterminados.
    QString ruta;
    // Carpeta donde el usuario puede editar la configuración.
    QString directorio;
};

class ConfiguracionBaseDatos
{
public:
    static QString rutaPortable(const QString& directorioAplicacion);
    static QString rutaUsuario();

    // Crea destino copiando plantilla. Devuelve false sin modificar nada si
    // destino ya existe o si la plantilla no puede leerse.
    static bool crearDesdePlantilla(const QString& destino,
                                    const QString& plantilla);

    static ConfiguracionResuelta resolver(const QString& directorioAplicacion,
                                          const QString& plantilla);

    static QString describir(OrigenConfiguracion origen);
};

#endif // CONFIGURACIONBASEDATOS_H
