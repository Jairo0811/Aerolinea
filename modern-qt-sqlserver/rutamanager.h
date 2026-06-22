#ifndef RUTAMANAGER_H
#define RUTAMANAGER_H

#include <QString>
#include <QStringList>
#include <QVector>

#include "destino.h"
#include "ruta.h"
#include "vuelo.h"
#include "aeronave.h"
class RutaManager
{
public:
    RutaManager();
    RutaManager(const QVector<Ruta>& rutas);
    RutaManager(const QVector<Ruta>& rutas, const QVector<Vuelo>& vuelos);
    RutaManager(const QVector<Ruta>& rutas,
                const QVector<Vuelo>& vuelos,
                const QVector<Aeronave>& aeronaves);
    QStringList obtenerDestinos() const;
    QString buscarRuta(const QString& origen, const QString& destino) const;

private:
    QVector<Destino> destinos;
    QVector<Ruta> rutas;
    QVector<Vuelo> vuelos;
QVector<Aeronave> aeronaves;
    bool existeDestino(const QString& nombre) const;
    QVector<Ruta> obtenerRutasDesde(const QString& origen) const;
    Vuelo buscarVueloPorRutaId(int rutaId) const;
    Aeronave buscarAeronavePorId(int aeronaveId) const;
};



#endif // RUTAMANAGER_H