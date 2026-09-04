#include "rutamanager.h"

#include <QHash>

#include <limits>
#include <queue>
#include <vector>

namespace
{
constexpr double INFINITO = std::numeric_limits<double>::infinity();

struct EstadoBusqueda
{
    QString destino;
    double costo;
};

struct MayorCosto
{
    bool operator()(const EstadoBusqueda& izquierda,
                    const EstadoBusqueda& derecha) const
    {
        return izquierda.costo > derecha.costo;
    }
};
}

RutaManager::RutaManager()
{
    rutas.append(Ruta(1, "Miami", "Orlando", 378, 70));
    rutas.append(Ruta(2, "Orlando", "Santo Domingo", 1600, 150));
    rutas.append(Ruta(3, "Santo Domingo", "San Francisco", 5400, 390));
    rutas.append(Ruta(4, "San Francisco", "Miami", 4150, 330));

    *this = RutaManager(rutas);
}

RutaManager::RutaManager(const QVector<Ruta>& rutas)
    : rutas(rutas)
{
    for (const Ruta& ruta : rutas) {
        if (!existeDestino(ruta.obtenerOrigen())) {
            destinos.append(Destino(destinos.size() + 1, ruta.obtenerOrigen()));
        }

        if (!existeDestino(ruta.obtenerDestino())) {
            destinos.append(Destino(destinos.size() + 1, ruta.obtenerDestino()));
        }
    }
}

RutaManager::RutaManager(const QVector<Ruta>& rutas,
                         const QVector<Vuelo>& vuelos)
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
        if (destino.obtenerNombre().compare(nombre, Qt::CaseInsensitive) == 0) {
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
        if (ruta.obtenerOrigen().compare(origen, Qt::CaseInsensitive) == 0) {
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

double RutaManager::costoTramo(const Ruta& ruta, CriterioRuta criterio) const
{
    switch (criterio) {
    case CriterioRuta::MenosEscalas:
        return 1.0;
    case CriterioRuta::MenorDistancia:
        return static_cast<double>(ruta.obtenerDistanciaKm());
    case CriterioRuta::MenorDuracion:
        return static_cast<double>(ruta.obtenerDuracionMinutos());
    case CriterioRuta::MenorPrecio: {
        const Vuelo vuelo = buscarVueloPorRutaId(ruta.obtenerId());
        return vuelo.obtenerId() != 0 ? vuelo.obtenerPrecio() : INFINITO;
    }
    }

    return INFINITO;
}

QVector<Ruta> RutaManager::calcularRuta(const QString& origen,
                                        const QString& destino,
                                        CriterioRuta criterio) const
{
    QVector<Ruta> vacia;

    if (origen.compare(destino, Qt::CaseInsensitive) == 0 ||
        !existeDestino(origen) || !existeDestino(destino)) {
        return vacia;
    }

    QHash<QString, double> costos;
    QHash<QString, Ruta> predecesores;

    for (const Destino& item : destinos) {
        costos.insert(item.obtenerNombre(), INFINITO);
    }

    QString origenCanonico;
    QString destinoCanonico;
    for (const Destino& item : destinos) {
        if (item.obtenerNombre().compare(origen, Qt::CaseInsensitive) == 0) {
            origenCanonico = item.obtenerNombre();
        }
        if (item.obtenerNombre().compare(destino, Qt::CaseInsensitive) == 0) {
            destinoCanonico = item.obtenerNombre();
        }
    }

    costos[origenCanonico] = 0.0;

    std::priority_queue<EstadoBusqueda,
                        std::vector<EstadoBusqueda>,
                        MayorCosto> pendientes;
    pendientes.push({origenCanonico, 0.0});

    while (!pendientes.empty()) {
        const EstadoBusqueda actual = pendientes.top();
        pendientes.pop();

        if (actual.costo > costos.value(actual.destino, INFINITO)) {
            continue;
        }

        if (actual.destino == destinoCanonico) {
            break;
        }

        for (const Ruta& ruta : obtenerRutasDesde(actual.destino)) {
            const double costoArista = costoTramo(ruta, criterio);
            if (costoArista == INFINITO) {
                continue;
            }

            const QString siguiente = ruta.obtenerDestino();
            const double nuevoCosto = actual.costo + costoArista;

            if (nuevoCosto < costos.value(siguiente, INFINITO)) {
                costos[siguiente] = nuevoCosto;
                predecesores.insert(siguiente, ruta);
                pendientes.push({siguiente, nuevoCosto});
            }
        }
    }

    if (!predecesores.contains(destinoCanonico)) {
        return vacia;
    }

    QVector<Ruta> rutaFinal;
    QString actual = destinoCanonico;

    while (actual != origenCanonico) {
        if (!predecesores.contains(actual)) {
            return vacia;
        }

        const Ruta tramo = predecesores.value(actual);
        rutaFinal.prepend(tramo);
        actual = tramo.obtenerOrigen();
    }

    return rutaFinal;
}

QString RutaManager::nombreCriterio(CriterioRuta criterio)
{
    switch (criterio) {
    case CriterioRuta::MenosEscalas:
        return "Menos escalas";
    case CriterioRuta::MenorDistancia:
        return "Menor distancia";
    case CriterioRuta::MenorDuracion:
        return "Menor duración";
    case CriterioRuta::MenorPrecio:
        return "Menor precio";
    }

    return "Menos escalas";
}

QString RutaManager::buscarRuta(const QString& origen,
                                const QString& destino,
                                CriterioRuta criterio) const
{
    if (origen.compare(destino, Qt::CaseInsensitive) == 0) {
        return "Estás en ese mismo destino.";
    }

    if (!existeDestino(origen) || !existeDestino(destino)) {
        return "El origen o destino no existe.";
    }

    const QVector<Ruta> rutaFinal = calcularRuta(origen, destino, criterio);
    if (rutaFinal.isEmpty()) {
        if (criterio == CriterioRuta::MenorPrecio) {
            return "No existe una ruta con precios de vuelo disponibles entre esos destinos.";
        }
        return "No existe una ruta disponible entre esos destinos.";
    }

    QString resultado = "Ruta encontrada\n";
    resultado += "Criterio: " + nombreCriterio(criterio) + "\n\n";

    int distanciaTotal = 0;
    int duracionTotal = 0;
    double precioTotal = 0.0;
    bool precioCompleto = true;

    for (const Ruta& tramo : rutaFinal) {
        resultado += tramo.obtenerOrigen() + " → " + tramo.obtenerDestino() + "\n";
        resultado += "Distancia: " + QString::number(tramo.obtenerDistanciaKm()) + " km\n";
        resultado += "Duración: " + QString::number(tramo.obtenerDuracionMinutos()) + " min\n";

        const Vuelo vuelo = buscarVueloPorRutaId(tramo.obtenerId());
        if (vuelo.obtenerId() != 0) {
            resultado += "Vuelo: " + vuelo.obtenerCodigoVuelo() + "\n";
            resultado += "Aerolínea: " + vuelo.obtenerAerolinea() + "\n";
            resultado += "Precio: US$" + QString::number(vuelo.obtenerPrecio(), 'f', 2) + "\n";
            precioTotal += vuelo.obtenerPrecio();

            const Aeronave aeronave = buscarAeronavePorId(vuelo.obtenerAeronaveId());
            if (aeronave.obtenerId() != 0) {
                resultado += "Aeronave: " + aeronave.obtenerModelo() + "\n";
                resultado += "Fabricante: " + aeronave.obtenerFabricante() + "\n";
                resultado += "Capacidad: " + QString::number(aeronave.obtenerCapacidad()) + " pasajeros\n";
            }
        } else {
            resultado += "Vuelo: No disponible\n";
            precioCompleto = false;
        }

        resultado += "\n";
        distanciaTotal += tramo.obtenerDistanciaKm();
        duracionTotal += tramo.obtenerDuracionMinutos();
    }

    const int escalas = rutaFinal.isEmpty() ? 0 : rutaFinal.size() - 1;
    resultado += "Resumen\n";
    resultado += "Escalas: " + QString::number(escalas) + "\n";
    resultado += "Distancia total: " + QString::number(distanciaTotal) + " km\n";
    resultado += "Duración total: " + QString::number(duracionTotal) + " min\n";
    resultado += precioCompleto
        ? "Precio total: US$" + QString::number(precioTotal, 'f', 2)
        : "Precio total: No disponible";

    return resultado;
}
