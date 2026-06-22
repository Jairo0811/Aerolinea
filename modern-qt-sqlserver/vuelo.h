#ifndef VUELO_H
#define VUELO_H

#include <QString>

class Vuelo
{
public:
    Vuelo();

    Vuelo(int id,
          int rutaId,
          int aeronaveId,
          const QString& codigoVuelo,
          const QString& aerolinea,
          double precio);

    int obtenerId() const;
    int obtenerRutaId() const;
    int obtenerAeronaveId() const;
    QString obtenerCodigoVuelo() const;
    QString obtenerAerolinea() const;
    double obtenerPrecio() const;

private:
    int id;
    int rutaId;
    int aeronaveId;
    QString codigoVuelo;
    QString aerolinea;
    double precio;
};

#endif // VUELO_H