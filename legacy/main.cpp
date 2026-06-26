#include <iostream>
#include <limits>
#include <string>

#include "paises.h"

using namespace std;

void limpiarPantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar()
{
    cout << "\nPresiona Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string leerTexto(const string& mensaje)
{
    string texto;
    cout << mensaje;
    getline(cin, texto);
    return texto;
}

void cargarDestinosIniciales(Paises& paises)
{
    paises.introducirDestino("miami");
    paises.introducirDestino("orlando");
    paises.introducirDestino("santo domingo");
    paises.introducirDestino("san francisco");
}

void mostrarEncabezado()
{
    cout << "=====================================\n";
    cout << "              AEROLINEA              \n";
    cout << "=====================================\n";
}

int main()
{
    Paises paises;
    cargarDestinosIniciales(paises);

    string opcion;

    do {
        limpiarPantalla();
        mostrarEncabezado();

        cout << "1. Introducir destino\n";
        cout << "2. Calcular cantidad de vuelos\n";
        cout << "3. Buscar ruta\n";
        cout << "4. Listar destinos\n";
        cout << "5. Listar rutas\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";

        getline(cin, opcion);
        limpiarPantalla();

        if (opcion == "1") {
            mostrarEncabezado();
            string destino = leerTexto("Ingrese el nuevo destino: ");
            paises.introducirDestino(destino);
            cout << "\nDestino agregado correctamente.\n";
            pausar();
        }
        else if (opcion == "2") {
            mostrarEncabezado();
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
            mostrarEncabezado();
            string origen = leerTexto("Ingrese el origen: ");
            string destino = leerTexto("Ingrese el destino: ");

            cout << "\nRuta encontrada:\n";
            cout << paises.buscarRuta(origen, destino) << endl;
            pausar();
        }
        else if (opcion == "4") {
            mostrarEncabezado();
            paises.listarDestinos();
            pausar();
        }
        else if (opcion == "5") {
            mostrarEncabezado();
            paises.listarRutas();
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
