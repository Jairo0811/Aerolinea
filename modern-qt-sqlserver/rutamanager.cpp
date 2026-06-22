#include "rutamanager.h"

RutaManager::RutaManager()
{
    rutas.append(Ruta(1, "Miami", "Orlando", 378, 70));
    rutas.append(Ruta(2, "Orlando", "Santo Domingo", 1600, 150));
    rutas.append(Ruta(3, "Santo Domingo", "San Francisco", 5400, 390));
    rutas.append(Ruta(4, "San Francisco", "Miami", 4150, 330));

    *this = RutaManager(rutas);
}

RutaManager::RutaManager(const QVector<Ruta>& rutas)
{
    this->rutas = rutas;

    for (const Ruta& ruta : rutas) {
        if (!existeDestino(ruta.obtenerOrigen())) {
            destinos.append(Destino(destinos.size() + 1, ruta.obtenerOrigen()));
        }

        if (!existeDestino(ruta.obtenerDestino())) {
            destinos.append(Destino(destinos.size() + 1, ruta.obtenerDestino()));
        }
    }
}

RutaManager::RutaManager(const QVector<Ruta>& rutas, const QVector<Vuelo>& vuelos)
    : RutaManager(rutas)
{
    this->vuelos = vuelos;
}

RutaManager::RutaManager(const QVector<Ruta>& rutas,
                         const QVector<Vuelo>& vuelos,
                         const QVector<Aeronave>& aeronaves)
    : RutaManager(rutas, vuelos)
{
    this->aeronaves = aeronaves;
}

bool RutaManager::existeDestino(const QString& nombre) const
{
    for (const Destino& destino : destinos) {
        if (destino.obtenerNombre() == nombre) {
            return true;
        }
    }

    return false;
}

QStringList RutaManager::obtenerDestinos() const
{
    QStringList lista;

    for (const Destino& destino : destinos) {
        lista << destino.obtenerNombre();
    }

    return lista;
}

QVector<Ruta> RutaManager::obtenerRutasDesde(const QString& origen) const
{
    QVector<Ruta> disponibles;

    for (const Ruta& ruta : rutas) {
        if (ruta.obtenerOrigen() == origen) {
            disponibles.append(ruta);
        }
    }

    return disponibles;
}

Vuelo RutaManager::buscarVueloPorRutaId(int rutaId) const
{
    for (const Vuelo& vuelo : vuelos) {
        if (vuelo.obtenerRutaId() == rutaId) {
            return vuelo;
        }
    }

    return Vuelo();
}

Aeronave RutaManager::buscarAeronavePorId(int aeronaveId) const
{
    for (const Aeronave& aeronave : aeronaves) {
        if (aeronave.obtenerId() == aeronaveId) {
            return aeronave;
        }
    }

    return Aeronave();
}

QString RutaManager::buscarRuta(const QString& origen, const QString& destino) const
{
    if (origen == destino) {
        return "Estás en ese mismo destino.";
    }

    if (!existeDestino(origen) || !existeDestino(destino)) {
        return "El origen o destino no existe.";
    }

    QVector<QString> cola;
    QVector<QString> visitados;
    QVector<Ruta> camino;

    cola.append(origen);
    visitados.append(origen);

    while (!cola.isEmpty()) {
        QString actual = cola.first();
        cola.removeFirst();

        QVector<Ruta> rutasDisponibles = obtenerRutasDesde(actual);

        for (const Ruta& ruta : rutasDisponibles) {
            QString siguiente = ruta.obtenerDestino();

            if (visitados.contains(siguiente)) {
                continue;
            }

            visitados.append(siguiente);
            cola.append(siguiente);
            camino.append(ruta);

            if (siguiente == destino) {
                QString resultado = "Ruta encontrada:\n\n";
                int distanciaTotal = 0;
                int duracionTotal = 0;
                double precioTotal = 0.0;

                QString reconstruir = destino;
                QVector<Ruta> rutaFinal;

                while (reconstruir != origen) {
                    bool encontrado = false;

                    for (int i = camino.size() - 1; i >= 0; --i) {
                        if (camino[i].obtenerDestino() == reconstruir) {
                            rutaFinal.prepend(camino[i]);
                            reconstruir = camino[i].obtenerOrigen();
                            encontrado = true;
                            break;
                        }
                    }

                    if (!encontrado) {
                        return "No se pudo reconstruir la ruta.";
                    }
                }

                for (const Ruta& tramo : rutaFinal) {
                    resultado += tramo.obtenerOrigen() + " → " + tramo.obtenerDestino() + "\n";
                    resultado += "Distancia: " + QString::number(tramo.obtenerDistanciaKm()) + " km\n";
                    resultado += "Duración: " + QString::number(tramo.obtenerDuracionMinutos()) + " min\n";

                    Vuelo vuelo = buscarVueloPorRutaId(tramo.obtenerId());

                    if (vuelo.obtenerId() != 0) {
                        resultado += "Vuelo: " + vuelo.obtenerCodigoVuelo() + "\n";
                        resultado += "Aerolínea: " + vuelo.obtenerAerolinea() + "\n";
                        resultado += "Precio: US$" + QString::number(vuelo.obtenerPrecio(), 'f', 2) + "\n";

                        precioTotal += vuelo.obtenerPrecio();

                        Aeronave aeronave = buscarAeronavePorId(vuelo.obtenerAeronaveId());

                        if (aeronave.obtenerId() != 0) {
                            resultado += "Aeronave: " + aeronave.obtenerModelo() + "\n";
                            resultado += "Fabricante: " + aeronave.obtenerFabricante() + "\n";
                            resultado += "Capacidad: " + QString::number(aeronave.obtenerCapacidad()) + " pasajeros\n";
                        }
                    } else {
                        resultado += "Vuelo: No disponible\n";
                    }

                    resultado += "\n";

                    distanciaTotal += tramo.obtenerDistanciaKm();
                    duracionTotal += tramo.obtenerDuracionMinutos();
                }

                int escalas = rutaFinal.size() > 0 ? rutaFinal.size() - 1 : 0;

                resultado += "Escalas: " + QString::number(escalas) + "\n";
                resultado += "Distancia total: " + QString::number(distanciaTotal) + " km\n";
                resultado += "Duración total: " + QString::number(duracionTotal) + " min\n";
                resultado += "Precio total: US$" + QString::number(precioTotal, 'f', 2);

                return resultado;
            }
        }
    }

    return "No existe una ruta disponible entre esos destinos.";
}