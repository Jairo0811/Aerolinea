#include <QtTest>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "configuracionbasedatos.h"

namespace
{
const QByteArray CONTENIDO_PLANTILLA =
    "[database]\nserver=plantilla\\\\SQLEXPRESS\n";

bool escribir(const QString& ruta, const QByteArray& contenido)
{
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile archivo(ruta);
    return archivo.open(QIODevice::WriteOnly) &&
           archivo.write(contenido) == contenido.size();
}

QByteArray leer(const QString& ruta)
{
    QFile archivo(ruta);
    return archivo.open(QIODevice::ReadOnly) ? archivo.readAll() : QByteArray();
}
}

class ConfiguracionTests : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir directorioAplicacion;
    QTemporaryDir directorioPlantilla;
    QString plantilla;

private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationName("AerolineaCPPTests");
        QCoreApplication::setApplicationName("ConfiguracionTests");
        QStandardPaths::setTestModeEnabled(true);

        QVERIFY(directorioAplicacion.isValid());
        QVERIFY(directorioPlantilla.isValid());
        plantilla = directorioPlantilla.filePath("database.example.ini");
        QVERIFY(escribir(plantilla, CONTENIDO_PLANTILLA));
    }

    void init()
    {
        // Estado limpio: sin configuración portable ni de usuario.
        QDir(directorioAplicacion.filePath("config")).removeRecursively();
        const QString usuario = ConfiguracionBaseDatos::rutaUsuario();
        QVERIFY(!usuario.isEmpty());
        QDir(QFileInfo(usuario).absolutePath()).removeRecursively();
    }

    void cleanupTestCase()
    {
        QDir(QFileInfo(ConfiguracionBaseDatos::rutaUsuario()).absolutePath())
            .removeRecursively();
    }

    void configuracionPortableTienePrioridad()
    {
        const QString portable =
            ConfiguracionBaseDatos::rutaPortable(directorioAplicacion.path());
        QVERIFY(escribir(portable, "[database]\nserver=portable\n"));
        QVERIFY(escribir(ConfiguracionBaseDatos::rutaUsuario(),
                         "[database]\nserver=usuario\n"));

        const ConfiguracionResuelta resultado = ConfiguracionBaseDatos::resolver(
            directorioAplicacion.path(), plantilla);

        QCOMPARE(resultado.origen, OrigenConfiguracion::Portable);
        QCOMPARE(resultado.ruta, portable);
        QCOMPARE(resultado.directorio, QFileInfo(portable).absolutePath());
    }

    void sinPortableSeCreaConfiguracionDeUsuarioDesdePlantilla()
    {
        const QString usuario = ConfiguracionBaseDatos::rutaUsuario();
        QVERIFY(!QFileInfo::exists(usuario));

        const ConfiguracionResuelta resultado = ConfiguracionBaseDatos::resolver(
            directorioAplicacion.path(), plantilla);

        QCOMPARE(resultado.origen, OrigenConfiguracion::Usuario);
        QCOMPARE(resultado.ruta, usuario);
        QCOMPARE(leer(usuario), CONTENIDO_PLANTILLA);
        QVERIFY(QFileInfo(usuario).isWritable());
    }

    void configuracionDeUsuarioExistenteNoSeSobrescribe()
    {
        const QString usuario = ConfiguracionBaseDatos::rutaUsuario();
        const QByteArray personalizada = "[database]\nserver=personalizado\n";
        QVERIFY(escribir(usuario, personalizada));

        const ConfiguracionResuelta resultado = ConfiguracionBaseDatos::resolver(
            directorioAplicacion.path(), plantilla);

        QCOMPARE(resultado.origen, OrigenConfiguracion::Usuario);
        QCOMPARE(leer(usuario), personalizada);
        QVERIFY(!ConfiguracionBaseDatos::crearDesdePlantilla(usuario, plantilla));
        QCOMPARE(leer(usuario), personalizada);
    }

    void plantillaJuntoAlEjecutableNoEsConfiguracionPortable()
    {
        QVERIFY(escribir(directorioAplicacion.filePath("config/database.example.ini"),
                         "[database]\nserver=ejemplo\n"));

        const ConfiguracionResuelta resultado = ConfiguracionBaseDatos::resolver(
            directorioAplicacion.path(), plantilla);

        QCOMPARE(resultado.origen, OrigenConfiguracion::Usuario);
        QCOMPARE(resultado.ruta, ConfiguracionBaseDatos::rutaUsuario());
    }

    void sinPlantillaSeUsanValoresPredeterminados()
    {
        const ConfiguracionResuelta resultado = ConfiguracionBaseDatos::resolver(
            directorioAplicacion.path(),
            directorioPlantilla.filePath("no-existe.ini"));

        QCOMPARE(resultado.origen, OrigenConfiguracion::Predeterminada);
        QVERIFY(resultado.ruta.isEmpty());
        QVERIFY(!QFileInfo::exists(ConfiguracionBaseDatos::rutaUsuario()));
        QCOMPARE(resultado.directorio,
                 QFileInfo(ConfiguracionBaseDatos::rutaUsuario()).absolutePath());
    }
};

QTEST_GUILESS_MAIN(ConfiguracionTests)
#include "configuracion_tests.moc"
