#ifndef NODO_H
#define NODO_H

#include <string>

struct Nodo {
    std::string nombre;
    Nodo* siguiente;

    explicit Nodo(const std::string& nombreDestino)
        : nombre(nombreDestino), siguiente(nullptr) {}
};

#endif // NODO_H
