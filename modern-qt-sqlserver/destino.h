#ifndef DESTINO_H
#define DESTINO_H

#include <QString>

class Destino
{
public:
    Destino();
    Destino(int id, const QString& nombre);

    int obtenerId() const;
    QString obtenerNombre() const;

private:
    int id;
    QString nombre;
};

#endif // DESTINO_H