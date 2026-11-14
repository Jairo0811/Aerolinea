#ifndef RUTAMANAGER_H
#define RUTAMANAGER_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <QVector>

#include "aeronave.h"
#include "destino.h"
#include "ruta.h"
#include "vuelo.h"

enum class CriterioRuta
{
    MenosEscalas = 0,
    MenorDistancia,
    MenorDuracion,
    MenorPrecio
};

class RutaManager
{
public:
    RutaManager();
    explicit RutaManager(const QVector<Ruta>& rutas);
    RutaManager(const QVector<Ruta>& rutas, const QVector<Vuelo>& vuelos);
    RutaManager(const QVector<Ruta>& rutas,
                const QVector<Vuelo>& vuelos,
                const QVector<Aeronave>& aeronaves);

    QStringList obtenerDestinos() const;
    QVector<Ruta> calcularRuta(
        const QString& origen,
        const QString& destino,
        CriterioRuta criterio = CriterioRuta::MenosEscalas) const;
    QString buscarRuta(
        const QString& origen,
        const QString& destino,
        CriterioRuta criterio = CriterioRuta::MenosEscalas) const;

    static QString nombreCriterio(CriterioRuta criterio);

private:
    QVector<Destino> destinos;
    QVector<Ruta> rutas;
    QVector<Vuelo> vuelos;
    QVector<Aeronave> aeronaves;
    // Vuelo con menor precio por RutaId (empate: menor Id de vuelo).
    QHash<int, Vuelo> mejorVueloPorRuta;

    bool existeDestino(const QString& nombre) const;
    QVector<Ruta> obtenerRutasDesde(const QString& origen) const;
    Vuelo buscarVueloPorRutaId(int rutaId) const;
    Aeronave buscarAeronavePorId(int aeronaveId) const;
    double costoTramo(const Ruta& ruta, CriterioRuta criterio) const;
};

#endif // RUTAMANAGER_H
