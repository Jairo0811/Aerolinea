#include "ruta.h"

Ruta::Ruta()
    : id(0),
    distanciaKm(0),
    duracionMinutos(0)
{
}

Ruta::Ruta(const QString& origen,
           const QString& destino,
           int distanciaKm,
           int duracionMinutos)
    : id(0),
    origen(origen),
    destino(destino),
    distanciaKm(distanciaKm),
    duracionMinutos(duracionMinutos)
{
}

Ruta::Ruta(int id,
           const QString& origen,
           const QString& destino,
           int distanciaKm,
           int duracionMinutos)
    : id(id),
    origen(origen),
    destino(destino),
    distanciaKm(distanciaKm),
    duracionMinutos(duracionMinutos)
{
}

int Ruta::obtenerId() const
{
    return id;
}

QString Ruta::obtenerOrigen() const
{
    return origen;
}

QString Ruta::obtenerDestino() const
{
    return destino;
}

int Ruta::obtenerDistanciaKm() const
{
    return distanciaKm;
}

int Ruta::obtenerDuracionMinutos() const
{
    return duracionMinutos;
}