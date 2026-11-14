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

    void destinoInexistenteSeRechaza()
    {
        const RutaManager manager = crearManager();
        QVERIFY(manager.calcularRuta("A", "Z").isEmpty());
        QCOMPARE(manager.buscarRuta("A", "Z"),
                 QString("El origen o destino no existe."));
    }

    void resumenAcumulaDistanciaDuracionPrecioYEscalas()
    {
        const RutaManager manager = crearManager();
        const QString resultado =
            manager.buscarRuta("A", "D", CriterioRuta::MenorDistancia);

        QVERIFY(resultado.contains("Escalas: 1\n"));
        QVERIFY(resultado.contains("Distancia total: 100 km\n"));
        QVERIFY(resultado.contains("Duración total: 600 min\n"));
        QVERIFY(resultado.endsWith("Precio total: US$10.00"));
    }

    void menorPrecioUsaElVueloMasBaratoDeCadaRuta()
    {
        // Varios vuelos pueden compartir RutaId; debe usarse el más barato,
        // no el primero recibido.
        const QVector<Ruta> rutas = {
            Ruta(1, "A", "B", 100, 60),
            Ruta(2, "B", "C", 100, 60),
            Ruta(3, "A", "C", 100, 60)
        };
        const QVector<Vuelo> vuelos = {
            Vuelo(1, 3, 1, "AC-CARO", "Test Air", 500.0),
            Vuelo(2, 1, 1, "AB", "Test Air", 50.0),
            Vuelo(3, 2, 1, "BC", "Test Air", 50.0),
            Vuelo(4, 3, 1, "AC-BARATO", "Test Air", 80.0),
            Vuelo(5, 3, 1, "AC-EMPATE", "Test Air", 80.0)
        };
        const RutaManager manager(rutas, vuelos);

        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "C", CriterioRuta::MenorPrecio);
        QCOMPARE(resultado.size(), 1);
        QCOMPARE(resultado.first().obtenerId(), 3);

        const QString texto = manager.buscarRuta("A", "C", CriterioRuta::MenorPrecio);
        QVERIFY(texto.contains("Vuelo: AC-BARATO\n"));
        QVERIFY(texto.endsWith("Precio total: US$80.00"));
    }

    void menorPrecioOmiteTramosSinVuelo()
    {
        const QVector<Ruta> rutas = {
            Ruta(1, "A", "B", 10, 10),
            Ruta(2, "B", "C", 10, 10),
            Ruta(3, "A", "C", 900, 900)
        };
        const QVector<Vuelo> vuelos = {
            Vuelo(1, 1, 1, "AB", "Test Air", 1.0),
            Vuelo(2, 3, 1, "AC", "Test Air", 300.0)
        };
        const RutaManager manager(rutas, vuelos);

        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "C", CriterioRuta::MenorPrecio);
        QCOMPARE(resultado.size(), 1);
        QCOMPARE(resultado.first().obtenerId(), 3);

        QVERIFY(manager.calcularRuta("B", "C", CriterioRuta::MenorPrecio).isEmpty());
        QCOMPARE(manager.buscarRuta("B", "C", CriterioRuta::MenorPrecio),
                 QString("No existe una ruta con precios de vuelo disponibles entre esos destinos."));

        const QString sinPrecio =
            manager.buscarRuta("A", "C", CriterioRuta::MenorDistancia);
        QVERIFY(sinPrecio.endsWith("Precio total: No disponible"));
    }

    void preciosNoValidosSeIgnoran()
    {
        const QVector<Ruta> rutas = {
            Ruta(1, "A", "B", 10, 10),
            Ruta(2, "B", "C", 10, 10),
            Ruta(3, "A", "C", 10, 10)
        };
        const QVector<Vuelo> vuelos = {
            Vuelo(1, 1, 1, "AB", "Test Air", 5.0),
            Vuelo(2, 2, 1, "BC", "Test Air", 5.0),
            Vuelo(3, 3, 1, "AC-NEG", "Test Air", -100.0)
        };
        const RutaManager manager(rutas, vuelos);

        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "C", CriterioRuta::MenorPrecio);
        QCOMPARE(resultado.size(), 2);
        QCOMPARE(resultado.at(0).obtenerId(), 1);
        QCOMPARE(resultado.at(1).obtenerId(), 2);
    }

    void capitalizacionDistintaEntreRutasFormaUnSoloNodo()
    {
        const QVector<Ruta> rutas = {
            Ruta(1, "Miami", "Orlando", 378, 70),
            Ruta(2, "orlando", "Santo Domingo", 1600, 150)
        };
        const RutaManager manager(rutas);

        QCOMPARE(manager.obtenerDestinos().size(), 3);
        for (const CriterioRuta criterio :
             {CriterioRuta::MenosEscalas, CriterioRuta::MenorDistancia,
              CriterioRuta::MenorDuracion}) {
            const QVector<Ruta> resultado =
                manager.calcularRuta("MIAMI", "santo domingo", criterio);
            QCOMPARE(resultado.size(), 2);
            QCOMPARE(resultado.at(0).obtenerId(), 1);
            QCOMPARE(resultado.at(1).obtenerId(), 2);
        }
    }

    void ciclosNoImpidenEncontrarLaRuta()
    {
        const QVector<Ruta> rutas = {
            Ruta(1, "A", "B", 10, 10),
            Ruta(2, "B", "A", 10, 10),
            Ruta(3, "B", "C", 10, 10),
            Ruta(4, "C", "A", 10, 10)
        };
        const RutaManager manager(rutas);

        const QVector<Ruta> resultado =
            manager.calcularRuta("A", "C", CriterioRuta::MenorDistancia);
        QCOMPARE(resultado.size(), 2);
        QCOMPARE(resultado.at(0).obtenerId(), 1);
        QCOMPARE(resultado.at(1).obtenerId(), 3);
    }

    void empatesSeResuelvenPorOrdenDeLasRutas()
    {
        // Dos caminos de igual costo: gana el primero descubierto según el
        // orden de las rutas, en cualquier compilador o plataforma.
        const QVector<Ruta> viaB = {
            Ruta(1, "A", "B", 10, 10),
            Ruta(2, "A", "C", 10, 10),
            Ruta(3, "B", "D", 10, 10),
            Ruta(4, "C", "D", 10, 10)
        };
        const QVector<Ruta> viaC = {
            Ruta(2, "A", "C", 10, 10),
            Ruta(1, "A", "B", 10, 10),
            Ruta(4, "C", "D", 10, 10),
            Ruta(3, "B", "D", 10, 10)
        };

        for (int repeticion = 0; repeticion < 3; ++repeticion) {
            const QVector<Ruta> resultadoB =
                RutaManager(viaB).calcularRuta("A", "D", CriterioRuta::MenorDistancia);
            QCOMPARE(resultadoB.size(), 2);
            QCOMPARE(resultadoB.at(0).obtenerId(), 1);
            QCOMPARE(resultadoB.at(1).obtenerId(), 3);

            const QVector<Ruta> resultadoC =
                RutaManager(viaC).calcularRuta("A", "D", CriterioRuta::MenorDistancia);
            QCOMPARE(resultadoC.size(), 2);
            QCOMPARE(resultadoC.at(0).obtenerId(), 2);
            QCOMPARE(resultadoC.at(1).obtenerId(), 4);
        }
    }
};

QTEST_APPLESS_MAIN(RutaManagerTests)
#include "rutamanager_tests.moc"
