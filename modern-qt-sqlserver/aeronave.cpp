#include "aeronave.h"

Aeronave::Aeronave()
    : id(0), capacidad(0)
{
}

Aeronave::Aeronave(int id, const QString& modelo, int capacidad, const QString& fabricante)
    : id(id), modelo(modelo), capacidad(capacidad), fabricante(fabricante)
{
}

int Aeronave::obtenerId() const
{
    return id;
}

QString Aeronave::obtenerModelo() const
{
    return modelo;
}

int Aeronave::obtenerCapacidad() const
{
    return capacidad;
}

QString Aeronave::obtenerFabricante() const
{
    return fabricante;
}