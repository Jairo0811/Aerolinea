#ifndef RUTA_H
#define RUTA_H

#include <QString>

class Ruta
{
public:
    Ruta();
    Ruta(const QString& origen,
         const QString& destino,
         int distanciaKm,
         int duracionMinutos);

    Ruta(int id,
         const QString& origen,
         const QString& destino,
         int distanciaKm,
         int duracionMinutos);

    int obtenerId() const;
    QString obtenerOrigen() const;
    QString obtenerDestino() const;
    int obtenerDistanciaKm() const;
    int obtenerDuracionMinutos() const;

private:
    int id;
    QString origen;
    QString destino;
    int distanciaKm;
    int duracionMinutos;
};

#endif // RUTA_H