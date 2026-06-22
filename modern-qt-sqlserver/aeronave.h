#ifndef AERONAVE_H
#define AERONAVE_H

#include <QString>

class Aeronave
{
public:
    Aeronave();
    Aeronave(int id, const QString& modelo, int capacidad, const QString& fabricante);

    int obtenerId() const;
    QString obtenerModelo() const;
    int obtenerCapacidad() const;
    QString obtenerFabricante() const;

private:
    int id;
    QString modelo;
    int capacidad;
    QString fabricante;
};

#endif // AERONAVE_H