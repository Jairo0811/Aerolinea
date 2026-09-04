#include <array>
#include <iostream>
#include <limits>
#include <string>

#include "paises.h"

using namespace std;

void limpiarPantalla()
{
    // Evita ejecutar un shell o un binario controlado mediante PATH.
    cout << "\x1B[2J\x1B[H" << flush;
}

void pausar()
{
    cout << "\nPresiona Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string leerTexto(const string& mensaje)
{
    cout << mensaje;
    array<char, 101> buffer{};
    cin.getline(buffer.data(), static_cast<streamsize>(buffer.size()));

    if (cin.fail() && !cin.eof()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return {};
    }

    return string(buffer.data());
}

void cargarDestinosIniciales(Paises& paises)
{
    paises.introducirDestino("Miami");
    paises.introducirDestino("Orlando");
    paises.introducirDestino("Santo Domingo");
    paises.introducirDestino("San Francisco");
}

void mostrarEncabezado(const Paises& paises)
{
    cout << "=====================================\n";
    cout << "           AEROLINEA (Legacy)        \n";
    cout << "=====================================\n";
    cout << "Destinos registrados: "
         << paises.cantidadDestinos()
         << "\n";
    cout << "=====================================\n\n";
}

int main()
{
    Paises paises;
    cargarDestinosIniciales(paises);

    string opcion;

    do
    {
        limpiarPantalla();
        mostrarEncabezado(paises);

        cout << "1. Introducir destino\n";
        cout << "2. Calcular cantidad de vuelos\n";
        cout << "3. Buscar ruta\n";
        cout << "4. Listar destinos\n";
        cout << "5. Listar rutas\n";
        cout << "0. Salir\n\n";

        opcion = leerTexto("Seleccione una opcion: ");

        limpiarPantalla();

        if (opcion == "1")
        {
            mostrarEncabezado(paises);

            string destino = leerTexto("Ingrese el nuevo destino: ");

            if (paises.introducirDestino(destino))
            {
                cout << "\nDestino agregado correctamente.\n";
            }
            else
            {
                cout << "\nEl destino ya existe o el nombre es invalido.\n";
            }

            pausar();
        }
        else if (opcion == "2")
        {
            mostrarEncabezado(paises);

            string origen = leerTexto("Ingrese el origen: ");
            string destino = leerTexto("Ingrese el destino: ");

            int vuelos = paises.contarVuelos(origen, destino);

            if (vuelos == -1)
            {
                cout << "\nNo existe una ruta valida entre esos destinos.\n";
            }
            else if (vuelos == 0)
            {
                cout << "\nYa se encuentra en ese destino.\n";
            }
            else
            {
                cout << "\nEsta a "
                     << vuelos
                     << " vuelo(s) de llegar a su destino.\n";
            }

            pausar();
        }
        else if (opcion == "3")
        {
            mostrarEncabezado(paises);

            string origen = leerTexto("Ingrese el origen: ");
            string destino = leerTexto("Ingrese el destino: ");

            cout << "\n";
            cout << paises.buscarRuta(origen, destino) << endl;

            pausar();
        }
        else if (opcion == "4")
        {
            mostrarEncabezado(paises);

            paises.listarDestinos();

            pausar();
        }
        else if (opcion == "5")
        {
            mostrarEncabezado(paises);

            paises.listarRutas();

            pausar();
        }
        else if (opcion == "0")
        {
            cout << "\nGracias por utilizar Aerolinea.\n";
        }
        else
        {
            cout << "\nOpcion invalida.\n";

            pausar();
        }

    }
    while (opcion != "0");

    return 0;
}
