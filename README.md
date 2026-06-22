# Aerolinea

Sistema de aerolínea desarrollado originalmente en abril de 2018 como proyecto final para la asignatura **Estructuras de Datos (SOF-012)** del **Instituto Tecnológico de Las Américas (ITLA)**.

El repositorio conserva la versión original en consola y una modernización realizada en 2026 con interfaz gráfica, SQL Server y arquitectura orientada a objetos.

## Integrantes del proyecto original

* Francis Jairo Matías Rosario - 2015-2984
* Jorge de Jesús Torres Pérez - 2016-3515
* Sebastian Donastor Hernández - 2016-3607

**Profesor:** Raydelto Hernández

## Estructura del repositorio

```text
Aerolinea/
├── legacy/
│   └── Versión original en C++ por consola
│
├── modern-qt-sqlserver/
│   └── Versión modernizada con Qt 6 + SQL Server
│
├── README.md
└── .gitignore
```

## Legacy

La carpeta `legacy` contiene el proyecto original desarrollado en C++ para consola durante abril de 2018.

## Modern Qt SQL Server

La carpeta `modern-qt-sqlserver` contiene una modernización del sistema utilizando:

* C++
* Qt 6 Widgets
* SQL Server
* ODBC
* CMake
* Programación Orientada a Objetos

## Funcionalidades de la versión modernizada

* Carga dinámica de destinos desde SQL Server
* Gestión de rutas aéreas
* Gestión de aeronaves
* Gestión de vuelos
* Búsqueda de rutas disponibles
* Cálculo de escalas
* Cálculo de distancia total
* Cálculo de duración total
* Cálculo de precio total
* Visualización de vuelo, aerolínea y aeronave
* Ventana "Acerca de" con información histórica del proyecto

## Base de datos

La base de datos incluye las tablas principales:

* Destinos
* Rutas
* Aeronaves
* Vuelos

El script SQL se encuentra en:

```text
modern-qt-sqlserver/Aerolinea.sql
```

## Objetivo de la modernización

Convertir un proyecto académico de consola en una aplicación de escritorio moderna, manteniendo la idea original y agregando una arquitectura más limpia, interfaz gráfica y persistencia de datos.

## Estado

**Versión modernizada funcional: v1.0**
