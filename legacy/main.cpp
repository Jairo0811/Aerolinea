#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct Nodo {
    string nombre;
    Nodo* siguiente;
};

class Paises {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    Paises() {
        cabeza = NULL;
        cola = NULL;
    }

    ~Paises() {
        Nodo* actual = cabeza;

        while (actual != NULL) {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }

    void introducirDestino(const string& pais) {
        Nodo* nuevo = new Nodo;
        nuevo->nombre = pais;
        nuevo->siguiente = NULL;

        if (cabeza == NULL) {
            cabeza = nuevo;
            cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
    }

    bool existeDestino(const string& pais) const {
        Nodo* temp = cabeza;

        while (temp != NULL) {
            if (temp->nombre == pais) {
                return true;
            }

            temp = temp->siguiente;
        }

        return false;
    }

    bool tieneOrigenYDestino(const string& origen, const string& destino) const {
        return existeDestino(origen) && existeDestino(destino);
    }

    int contarVuelos(const string& origen, const string& destino) const {
        if (!tieneOrigenYDestino(origen, destino)) {
            return -1;
        }

        if (origen == destino) {
            return 0;
        }

        Nodo* temp = cabeza;
        bool contando = false;
        int vuelos = 0;

        while (temp != NULL) {
            if (temp->nombre == origen) {
                contando = true;
            }

            if (contando && temp->siguiente != NULL) {
                vuelos++;
            }

            if (contando && temp->siguiente != NULL && temp->siguiente->nombre == destino) {
                return vuelos;
            }

            temp = temp->siguiente;
        }

        // Ruta circular: último destino conecta con el primero.
        if (contando && cola != NULL && cabeza != NULL) {
            vuelos++;

            if (cabeza->nombre == destino) {
                return vuelos;
            }

            temp = cabeza;

            while (temp != NULL && temp->nombre != origen) {
                if (temp->siguiente != NULL && temp->siguiente->nombre == destino) {
                    return vuelos + 1;
                }

                vuelos++;
                temp = temp->siguiente;
            }
        }

        return -1;
    }

    string buscarRuta(const string& origen, const string& destino) const {
        if (!tieneOrigenYDestino(origen, destino)) {
            return "El origen o el destino ingresado no existe.";
        }

        if (origen == destino) {
            return "Ya estas en ese destino.";
        }

        string ruta = "";
        Nodo* temp = cabeza;
        bool construyendo = false;

        while (temp != NULL) {
            if (temp->nombre == origen) {
                construyendo = true;
            }

            if (construyendo && temp->siguiente != NULL) {
                ruta += temp->nombre + " hacia " + temp->siguiente->nombre + "\n";

                if (temp->siguiente->nombre == destino) {
                    return ruta;
                }
            }

            temp = temp->siguiente;
        }

        // Ruta circular: último destino conecta con el primero.
        if (construyendo && cola != NULL && cabeza != NULL) {
            ruta += cola->nombre + " hacia " + cabeza->nombre + "\n";

            if (cabeza->nombre == destino) {
                return ruta;
            }

            temp = cabeza;

            while (temp != NULL && temp->nombre != origen) {
                if (temp->siguiente != NULL) {
                    ruta += temp->nombre + " hacia " + temp->siguiente->nombre + "\n";

                    if (temp->siguiente->nombre == destino) {
                        return ruta;
                    }
                }

                temp = temp->siguiente;
            }
        }

        return "No se encontro una ruta disponible.";
    }

    void listarDestinos() const {
        Nodo* temp = cabeza;

        cout << "\nDestinos registrados:\n";

        while (temp != NULL) {
            cout << "- " << temp->nombre << endl;
            temp = temp->siguiente;
        }

        cout << endl;
    }
};

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\nPresiona Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string leerTexto(const string& mensaje) {
    string texto;

    cout << mensaje;
    getline(cin, texto);

    return texto;
}

int main() {
    Paises paises;

    paises.introducirDestino("miami");
    paises.introducirDestino("orlando");
    paises.introducirDestino("santo domingo");
    paises.introducirDestino("san francisco");

    string opcion;

    do {
        limpiarPantalla();

        cout << "=====================================\n";
        cout << "              AEROLINEA\n";
        cout << "=====================================\n";
        cout << "1. Introducir destino\n";
        cout << "2. Calcular cantidad de vuelos\n";
        cout << "3. Buscar ruta\n";
        cout << "4. Listar destinos\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";

        getline(cin, opcion);

        limpiarPantalla();

        if (opcion == "1") {
            string destino = leerTexto("Ingrese el nuevo destino: ");
            paises.introducirDestino(destino);

            cout << "\nDestino agregado correctamente.\n";
            pausar();
        }
        else if (opcion == "2") {
            string origen = leerTexto("Ingrese el origen: ");
            string destino = leerTexto("Ingrese el destino: ");

            int vuelos = paises.contarVuelos(origen, destino);

            if (vuelos == -1) {
                cout << "\nNo existe una ruta valida entre esos destinos.\n";
            } else {
                cout << "\nEsta a " << vuelos << " vuelo(s) de llegar a su destino.\n";
            }

            pausar();
        }
        else if (opcion == "3") {
            string origen = leerTexto("Ingrese el origen: ");
            string destino = leerTexto("Ingrese el destino: ");

            cout << "\n" << paises.buscarRuta(origen, destino) << endl;
            pausar();
        }
        else if (opcion == "4") {
            paises.listarDestinos();
            pausar();
        }
        else if (opcion == "0") {
            cout << "\nSaliendo del sistema...\n";
        }
        else {
            cout << "\nOpcion invalida.\n";
            pausar();
        }

    } while (opcion != "0");

    return 0;
}