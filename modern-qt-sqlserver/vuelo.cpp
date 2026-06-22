#include "vuelo.h"

Vuelo::Vuelo()
    : id(0),
    rutaId(0),
    aeronaveId(0),
    precio(0.0)
{
}

Vuelo::Vuelo(int id,
             int rutaId,
             int aeronaveId,
             const QString& codigoVuelo,
             const QString& aerolinea,
             double precio)
    : id(id),
    rutaId(rutaId),
    aeronaveId(aeronaveId),
    codigoVuelo(codigoVuelo),
    aerolinea(aerolinea),
    precio(precio)
{
}

int Vuelo::obtenerId() const
{
    return id;
}

int Vuelo::obtenerRutaId() const
{
    return rutaId;
}

int Vuelo::obtenerAeronaveId() const
{
    return aeronaveId;
}

QString Vuelo::obtenerCodigoVuelo() const
{
    return codigoVuelo;
}

QString Vuelo::obtenerAerolinea() const
{
    return aerolinea;
}

double Vuelo::obtenerPrecio() const
{
    return precio;
}