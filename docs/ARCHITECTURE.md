# Arquitectura de AerolineaCPP

Aerolinea conserva dos etapas deliberadamente separadas: el **proyecto académico Legacy de 2018**, basado en consola y listas enlazadas, y la **modernización AerolineaCPP de 2026**, construida con C++17, Qt 6 Widgets y SQL Server. La arquitectura moderna no reemplaza ni altera el valor histórico del código Legacy.

## Vista general de AerolineaCPP

```mermaid
flowchart LR
    User["Usuario"] --> UI["Qt 6 Widgets · MainWindow"]
    UI --> Route["RutaManager"]
    UI --> DBM["DatabaseManager"]

    Route --> Domain["Ruta · Vuelo · Destino · Aeronave"]
    Route --> DBM
    DBM --> QtSql["Qt SQL / ODBC"]
    QtSql --> SQL[("SQL Server 2022")]

    Config["config/database.ini"] --> DBM
    Resources["Qt Resources / Assets"] --> UI
```

`MainWindow` coordina presentación e interacción. `RutaManager` concentra la lógica de búsqueda y composición de rutas. `DatabaseManager` encapsula la conexión y consultas a SQL Server, evitando que la interfaz manipule directamente detalles de ODBC.

## Separación de responsabilidades

```mermaid
flowchart TD
    Presentation["Presentation · Qt Widgets"] --> Application["Application · RutaManager"]
    Application --> Domain["Domain · Entidades"]
    Application --> Infrastructure["Infrastructure · DatabaseManager"]
    Infrastructure --> DB[("SQL Server")]
    Config["Configuración externa"] --> Infrastructure
```

| Componente | Responsabilidad |
|---|---|
| `MainWindow` | Interfaz, comandos, selección de destinos y presentación de resultados |
| `RutaManager` | Búsqueda de rutas directas/con escalas y cálculo agregado del recorrido |
| `Ruta`, `Vuelo`, `Destino`, `Aeronave` | Modelo de dominio de la solución |
| `DatabaseManager` | Apertura de conexión, ejecución de consultas y mapeo de datos |
| `database.ini` | Configuración externa de la instancia SQL Server |
| Qt Resources | Logo, iconos y recursos visuales incluidos en el ejecutable |

## Búsqueda de rutas

```mermaid
sequenceDiagram
    participant U as Usuario
    participant W as MainWindow
    participant R as RutaManager
    participant D as DatabaseManager
    participant DB as SQL Server

    U->>W: origen + destino
    W->>R: buscarRuta(origen, destino)
    R->>D: consultar destinos, vuelos y rutas
    D->>DB: consultas SQL vía ODBC
    DB-->>D: registros
    D-->>R: objetos de dominio
    R->>R: calcular escalas, distancia, duración y precio
    R-->>W: itinerario resultante
    W-->>U: mostrar ruta / estado / error
```

La lógica de cálculo se mantiene fuera de la ventana para que la interfaz no sea responsable de construir itinerarios ni sumar métricas del viaje.

## Persistencia

```mermaid
flowchart LR
    App["AerolineaCPP"] --> DBM["DatabaseManager"]
    DBM --> Driver["ODBC Driver"]
    Driver --> SQL[("SQL Server")]
    SQL --> Destinos["Destinos"]
    SQL --> Rutas["Rutas"]
    SQL --> Aeronaves["Aeronaves"]
    SQL --> Vuelos["Vuelos"]
```

La conexión se configura externamente para evitar acoplar el ejecutable a una instancia específica. Las credenciales locales no deben versionarse.

## Build y distribución

```mermaid
flowchart LR
    Source["C++17 / Qt 6"] --> CMake["CMake"]
    CMake --> Build["Build Windows x64"]
    Build --> Deploy["Qt deployment / DLLs / Plugins"]
    Config["database.example.ini"] --> Package["Paquete portable"]
    SQLScript["Aerolinea.sql"] --> Package
    Deploy --> Package
    Actions["GitHub Actions"] --> Build
    Package --> Release["GitHub Releases"]
```

## Relación con Legacy

```mermaid
flowchart TB
    Academic["2018-C1 · Proyecto académico"] --> Legacy["legacy/ · C++ consola / listas enlazadas"]
    Academic --> Concept["Concepto: rutas y estructuras de datos"]
    Concept --> Modern["2026 · AerolineaCPP"]
    Modern --> Qt["Qt 6 Widgets"]
    Modern --> SQL["SQL Server"]
    Modern --> CMake["CMake / Release portable"]
```

Legacy y AerolineaCPP comparten contexto académico y concepto funcional, pero permanecen técnicamente separados. Esto permite comparar la evolución del proyecto sin reescribir retrospectivamente el código original.

## Criterio de evolución

La aplicación debe conservar una separación clara entre UI, lógica de rutas y acceso a datos. Nuevas integraciones —por ejemplo APIs aeronáuticas externas— deberían incorporarse como servicios independientes detrás de interfaces, no directamente en `MainWindow`.
