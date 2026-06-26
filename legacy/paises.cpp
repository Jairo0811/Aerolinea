#include "paises.h"

#include <iostream>

using namespace std;

Paises::Paises()
    : cabeza(nullptr), cola(nullptr)
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
}

void Paises::introducirDestino(const string& pais)
{
    if (pais.empty()) {
        return;
    }

    Nodo* nuevo = new Nodo(pais);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        cola = nuevo;
    }
}

bool Paises::existeDestino(const string& pais) const
{
    Nodo* temp = cabeza;

    while (temp != nullptr) {
        if (temp->nombre == pais) {
            return true;
        }

        temp = temp->siguiente;
    }

    return false;
}

bool Paises::tieneOrigenYDestino(const string& origen, const string& destino) const
{
    return existeDestino(origen) && existeDestino(destino);
}

int Paises::contarVuelos(const string& origen, const string& destino) const
{
    if (!tieneOrigenYDestino(origen, destino)) {
        return -1;
    }

    if (origen == destino) {
        return 0;
    }

    Nodo* temp = cabeza;
    bool contando = false;
    int vuelos = 0;

    while (temp != nullptr) {
        if (temp->nombre == origen) {
            contando = true;
        }

        if (contando && temp->siguiente != nullptr) {
            vuelos++;

            if (temp->siguiente->nombre == destino) {
                return vuelos;
            }
        }

        temp = temp->siguiente;
    }

    // Ruta circular: el último destino conecta con el primero.
    if (contando && cola != nullptr && cabeza != nullptr) {
        vuelos++;

        if (cabeza->nombre == destino) {
            return vuelos;
        }

        temp = cabeza;

        while (temp != nullptr && temp->nombre != origen) {
            if (temp->siguiente != nullptr) {
                vuelos++;

                if (temp->siguiente->nombre == destino) {
                    return vuelos;
                }
            }

            temp = temp->siguiente;
        }
    }

    return -1;
}

string Paises::buscarRuta(const string& origen, const string& destino) const
{
    if (!tieneOrigenYDestino(origen, destino)) {
        return "El origen o el destino ingresado no existe.";
    }

    if (origen == destino) {
        return "Ya estas en ese destino.";
    }

    string ruta;
    Nodo* temp = cabeza;
    bool construyendo = false;

    while (temp != nullptr) {
        if (temp->nombre == origen) {
            construyendo = true;
        }

        if (construyendo && temp->siguiente != nullptr) {
            ruta += temp->nombre + " -> " + temp->siguiente->nombre + "\n";

            if (temp->siguiente->nombre == destino) {
                return ruta;
            }
        }

        temp = temp->siguiente;
    }

    // Ruta circular: el último destino conecta con el primero.
    if (construyendo && cola != nullptr && cabeza != nullptr) {
        ruta += cola->nombre + " -> " + cabeza->nombre + "\n";

        if (cabeza->nombre == destino) {
            return ruta;
        }

        temp = cabeza;

        while (temp != nullptr && temp->nombre != origen) {
            if (temp->siguiente != nullptr) {
                ruta += temp->nombre + " -> " + temp->siguiente->nombre + "\n";

                if (temp->siguiente->nombre == destino) {
                    return ruta;
                }
            }

            temp = temp->siguiente;
        }
    }

    return "No se encontro una ruta disponible.";
}

void Paises::listarDestinos() const
{
    Nodo* temp = cabeza;

    cout << "\nDestinos registrados:\n";

    while (temp != nullptr) {
        cout << "- " << temp->nombre << endl;
        temp = temp->siguiente;
    }
}

void Paises::listarRutas() const
{
    Nodo* temp = cabeza;

    cout << "\nRutas registradas:\n";

    while (temp != nullptr && temp->siguiente != nullptr) {
        cout << "- " << temp->nombre << " -> " << temp->siguiente->nombre << endl;
        temp = temp->siguiente;
    }

    if (cola != nullptr && cabeza != nullptr && cola != cabeza) {
        cout << "- " << cola->nombre << " -> " << cabeza->nombre << endl;
    }
}
