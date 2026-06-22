#include "destino.h"

Destino::Destino()
    : id(0)
{
}

Destino::Destino(int id, const QString& nombre)
    : id(id),
    nombre(nombre)
{
}

int Destino::obtenerId() const
{
    return id;
}

QString Destino::obtenerNombre() const
{
    return nombre;
}
