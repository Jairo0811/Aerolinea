#include "paises.h"

#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

string Paises::limpiarEspacios(const string& texto)
{
    const auto inicio = find_if(
        texto.begin(),
        texto.end(),
        [](unsigned char caracter)
        {
            return !isspace(caracter);
        }
    );

    if (inicio == texto.end()) {
        return "";
    }

    const auto fin = find_if(
        texto.rbegin(),
        texto.rend(),
        [](unsigned char caracter)
        {
            return !isspace(caracter);
        }
    ).base();

    return string(inicio, fin);
}

string Paises::normalizar(const string& texto)
{
    string resultado = limpiarEspacios(texto);

    transform(
        resultado.begin(),
        resultado.end(),
        resultado.begin(),
        [](unsigned char caracter)
        {
            return static_cast<char>(tolower(caracter));
        }
    );

    return resultado;
}

Paises::Paises()
    : cabeza(nullptr),
      cola(nullptr)
{
}

Paises::~Paises()
{
    Nodo* actual = cabeza;

    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;

        delete actual;
        actual = siguiente;
    }

    cabeza = nullptr;
    cola = nullptr;
}

bool Paises::introducirDestino(const string& pais)
{
    const string paisLimpio = limpiarEspacios(pais);

    if (paisLimpio.empty()) {
        return false;
    }

    if (existeDestino(paisLimpio)) {
        return false;
    }

    Nodo* nuevo = new Nodo(paisLimpio);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        cola = nuevo;
    }

    return true;
}

bool Paises::existeDestino(const string& pais) const
{
    const string paisNormalizado = normalizar(pais);

    if (paisNormalizado.empty()) {
        return false;
    }

    Nodo* actual = cabeza;

    while (actual != nullptr) {
        if (normalizar(actual->nombre) == paisNormalizado) {
            return true;
        }

        actual = actual->siguiente;
    }

    return false;
}

bool Paises::tieneOrigenYDestino(
    const string& origen,
    const string& destino
) const
{
    return existeDestino(origen) && existeDestino(destino);
}

int Paises::cantidadDestinos() const
{
    int cantidad = 0;
    Nodo* actual = cabeza;

    while (actual != nullptr) {
        ++cantidad;
        actual = actual->siguiente;
    }

    return cantidad;
}

int Paises::contarVuelos(
    const string& origen,
    const string& destino
) const
{
    if (!tieneOrigenYDestino(origen, destino)) {
        return -1;
    }

    const string origenNormalizado = normalizar(origen);
    const string destinoNormalizado = normalizar(destino);

    if (origenNormalizado == destinoNormalizado) {
        return 0;
    }

    Nodo* actual = cabeza;

    while (
        actual != nullptr &&
        normalizar(actual->nombre) != origenNormalizado
    ) {
        actual = actual->siguiente;
    }

    if (actual == nullptr) {
        return -1;
    }

    const int totalDestinos = cantidadDestinos();
    int vuelos = 0;

    for (int i = 0; i < totalDestinos; ++i) {
        Nodo* siguiente = actual->siguiente;

        if (siguiente == nullptr) {
            siguiente = cabeza;
        }

        ++vuelos;

        if (normalizar(siguiente->nombre) == destinoNormalizado) {
            return vuelos;
        }

        actual = siguiente;
    }

    return -1;
}

string Paises::buscarRuta(
    const string& origen,
    const string& destino
) const
{
    if (!tieneOrigenYDestino(origen, destino)) {
        return "El origen o el destino ingresado no existe.";
    }

    const string origenNormalizado = normalizar(origen);
    const string destinoNormalizado = normalizar(destino);

    if (origenNormalizado == destinoNormalizado) {
        return "Ya estas en ese destino.";
    }

    Nodo* actual = cabeza;

    while (
        actual != nullptr &&
        normalizar(actual->nombre) != origenNormalizado
    ) {
        actual = actual->siguiente;
    }

    if (actual == nullptr) {
        return "No se encontro una ruta disponible.";
    }

    string ruta;
    const int totalDestinos = cantidadDestinos();

    for (int i = 0; i < totalDestinos; ++i) {
        Nodo* siguiente = actual->siguiente;

        if (siguiente == nullptr) {
            siguiente = cabeza;
        }

        ruta += actual->nombre;
        ruta += " -> ";
        ruta += siguiente->nombre;
        ruta += "\n";

        if (normalizar(siguiente->nombre) == destinoNormalizado) {
            return ruta;
        }

        actual = siguiente;
    }

    return "No se encontro una ruta disponible.";
}

void Paises::listarDestinos() const
{
    if (cabeza == nullptr) {
        cout << "\nNo hay destinos registrados.\n";
        return;
    }

    cout << "\nDestinos registrados ("
         << cantidadDestinos()
         << "):\n";

    Nodo* actual = cabeza;
    int numero = 1;

    while (actual != nullptr) {
        cout << numero << ". " << actual->nombre << '\n';

        ++numero;
        actual = actual->siguiente;
    }
}

void Paises::listarRutas() const
{
    if (cabeza == nullptr) {
        cout << "\nNo hay rutas registradas.\n";
        return;
    }

    if (cabeza == cola) {
        cout << "\nSolo existe un destino registrado.\n";
        return;
    }

    cout << "\nRutas registradas:\n";

    Nodo* actual = cabeza;

    while (actual != nullptr && actual->siguiente != nullptr) {
        cout << "- "
             << actual->nombre
             << " -> "
             << actual->siguiente->nombre
             << '\n';

        actual = actual->siguiente;
    }

    cout << "- "
         << cola->nombre
         << " -> "
         << cabeza->nombre
         << '\n';
}