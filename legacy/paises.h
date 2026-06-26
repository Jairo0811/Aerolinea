#ifndef PAISES_H
#define PAISES_H

#include <string>
#include "nodo.h"

class Paises {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    Paises();
    ~Paises();

    void introducirDestino(const std::string& pais);
    bool existeDestino(const std::string& pais) const;
    bool tieneOrigenYDestino(const std::string& origen, const std::string& destino) const;
    int contarVuelos(const std::string& origen, const std::string& destino) const;
    std::string buscarRuta(const std::string& origen, const std::string& destino) const;
    void listarDestinos() const;
    void listarRutas() const;
};

#endif // PAISES_H
