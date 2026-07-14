#ifndef PAISES_H
#define PAISES_H

#include <string>

#include "nodo.h"

class Paises
{
private:
    Nodo* cabeza;
    Nodo* cola;

    static std::string limpiarEspacios(const std::string& texto);
    static std::string normalizar(const std::string& texto);

public:
    Paises();
    ~Paises();

    // Evita que la lista sea copiada accidentalmente.
    Paises(const Paises&) = delete;
    Paises& operator=(const Paises&) = delete;

    bool introducirDestino(const std::string& pais);

    bool existeDestino(const std::string& pais) const;

    bool tieneOrigenYDestino(
        const std::string& origen,
        const std::string& destino
    ) const;

    int contarVuelos(
        const std::string& origen,
        const std::string& destino
    ) const;

    std::string buscarRuta(
        const std::string& origen,
        const std::string& destino
    ) const;

    int cantidadDestinos() const;

    void listarDestinos() const;
    void listarRutas() const;
};

#endif // PAISES_H