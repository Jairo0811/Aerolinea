#include <QtTest>

#include "rutamanager.h"

class RutaManagerTests : public QObject
{
    Q_OBJECT

private:
    static RutaManager crearManager()
    {
        const QVector<Ruta> rutas = {
            Ruta(1, "A", "B", 100, 100),
            Ruta(2, "B", "D", 100, 100),
            Ruta(3, "A", "D", 500, 50),
            Ruta(4, "A", "C", 50, 300),
            Ruta(5, "C", "D", 50, 300)
        };

        const QVector<Vuelo> vuelos = {
            Vuelo(1, 1, 1, "AB100", "Test Air", 10.0),
            Vuelo(2, 2, 1, "BD100", "Test Air", 10.0),
            Vuelo(3, 3, 1, "AD500", "Test Air", 100.0),
            Vuelo(4, 4, 1, "AC050", "Test Air", 5.0),
            Vuelo(5, 5, 1, "CD050", "Test Air", 5.0)
        };

        return RutaManager(rutas, vuelos);
    }

private slots:
    void menosEscalasPrefiereRutaDirecta()
    {
        const RutaManager manager = crearManager();
        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "D", CriterioRuta::MenosEscalas);

        QCOMPARE(resultado.size(), 1);
        QCOMPARE(resultado.first().obtenerId(), 3);
    }

    void menorDistanciaUsaCaminoMasCorto()
    {
        const RutaManager manager = crearManager();
        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "D", CriterioRuta::MenorDistancia);

        QCOMPARE(resultado.size(), 2);
        QCOMPARE(resultado.at(0).obtenerId(), 4);
        QCOMPARE(resultado.at(1).obtenerId(), 5);
    }

    void menorDuracionPrefiereRutaDirecta()
    {
        const RutaManager manager = crearManager();
        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "D", CriterioRuta::MenorDuracion);

        QCOMPARE(resultado.size(), 1);
        QCOMPARE(resultado.first().obtenerId(), 3);
    }

    void menorPrecioUsaVuelosMasBaratos()
    {
        const RutaManager manager = crearManager();
        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "D", CriterioRuta::MenorPrecio);

        QCOMPARE(resultado.size(), 2);
        QCOMPARE(resultado.at(0).obtenerId(), 4);
        QCOMPARE(resultado.at(1).obtenerId(), 5);
    }

    void destinoInalcanzableDevuelveRutaVacia()
    {
        const RutaManager manager = crearManager();
        QVERIFY(manager.calcularRuta("D", "A").isEmpty());
    }

    void origenYDestinoIgualesSeRechazan()
    {
        const RutaManager manager = crearManager();
        QCOMPARE(manager.buscarRuta("A", "A"), QString("Estás en ese mismo destino."));
    }

    void busquedaEsCaseInsensitive()
    {
        const RutaManager manager = crearManager();
        const QVector<Ruta> resultado =
            manager.calcularRuta("a", "d", CriterioRuta::MenorDuracion);

        QCOMPARE(resultado.size(), 1);
        QCOMPARE(resultado.first().obtenerId(), 3);
    }
};

QTEST_APPLESS_MAIN(RutaManagerTests)
#include "rutamanager_tests.moc"
