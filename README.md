<p align="center">
  <img src="modern-qt-sqlserver/Arolinea-CPP.png" width="420" alt="Logo de AerolineaCPP">
</p>
<p align="center">
  <img src="https://img.shields.io/badge/ITLA-2018--C1-0057B8?style=for-the-badge" alt="ITLA 2018-C1" />
</p>


<p align="center">

  <img src="https://img.shields.io/badge/Estado-Finalizado-success?style=for-the-badge" alt="Estado finalizado" />
  <img src="https://img.shields.io/github/v/release/Jairo0811/Aerolinea?style=for-the-badge&label=Release" alt="GitHub Release" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6-41CD52?style=for-the-badge&logo=qt&logoColor=white" alt="Qt 6" />
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17" />
  <img src="https://img.shields.io/badge/SQL%20Server-2022-CC2927?style=for-the-badge&logo=microsoftsqlserver&logoColor=white" alt="SQL Server" />
  <img src="https://img.shields.io/badge/CMake-Build-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake" />
</p>

<p align="center">
  <strong>De un proyecto académico de consola de 2018 a una aplicación de escritorio moderna en 2026.</strong>
</p>

<p align="center">
  <a href="https://github.com/Jairo0811/Aerolinea/releases/tag/v1.0.0"><strong>📦 Descargar AerolineaCPP v1.0.0</strong></a>
  ·
  <a href="https://github.com/Jairo0811/Aerolinea/releases"><strong>🚀 Releases</strong></a>
</p>

> **Estado actual:** proyecto finalizado, restaurado, modernizado, distribuible para Windows x64 y documentado para portafolio profesional.

---

## 📌 Descripción

**Aerolinea** es un sistema para la gestión y búsqueda de rutas aéreas desarrollado originalmente como **proyecto final** de la asignatura **Estructuras de Datos (SOF-012)** durante el **cuatrimestre 2018-C1** del **Instituto Tecnológico de Las Américas (ITLA)**.

En **2026**, el proyecto fue recuperado con dos objetivos: **preservar su versión académica original** y demostrar su evolución mediante una modernización construida con **C++17, Qt 6 Widgets, Microsoft SQL Server, ODBC y CMake**.

El repositorio conserva ambas etapas del proyecto:

- 🕰️ **Legacy (2018-C1):** aplicación original de consola basada en listas enlazadas.
- 🚀 **AerolineaCPP (2026):** aplicación de escritorio moderna, responsive y respaldada por SQL Server.

La modernización no sustituye el proyecto histórico: ambos conviven en el mismo repositorio para hacer visible el proceso de evolución técnica.

---

## ✨ AerolineaCPP v1.0.0

La versión moderna ofrece una interfaz de escritorio en tema oscuro, identidad visual propia y un flujo centrado en consultar rutas aéreas almacenadas en SQL Server.

### Funcionalidades principales

- 🌎 Carga dinámica de destinos desde SQL Server.
- 🛫 Gestión y consulta de rutas aéreas.
- ✈️ Gestión de aeronaves.
- 🎫 Gestión de vuelos.
- 🔎 Búsqueda de rutas directas y con escalas.
- 🔄 Cálculo automático del número de escalas.
- 📏 Cálculo de distancia total del recorrido.
- ⏱️ Cálculo de duración total.
- 💵 Cálculo del precio total del viaje.
- 🛩️ Información de aerolínea, vuelo y aeronave para cada tramo.
- 🟢 Estados visuales para búsquedas exitosas, errores y rutas no disponibles.
- 🧹 Acción para limpiar rápidamente una consulta.
- 📊 Indicadores de destinos, rutas, aeronaves y vuelos registrados.
- 📋 Ventana **Acerca de** con la historia y créditos del proyecto original.
- 🏛️ Identidad académica del ITLA preservada dentro de la aplicación.
- 🖼️ Recursos visuales embebidos y fallback externo para la distribución portable.
- ⚙️ Configuración externa de la conexión mediante `database.ini`.

---

## 📦 Release estable para Windows

La versión **v1.0.0** se distribuye como paquete portable para **Windows x64** mediante GitHub Releases.

### Descarga

**[AerolineaCPP v1.0.0 — Windows x64](https://github.com/Jairo0811/Aerolinea/releases/tag/v1.0.0)**

El paquete publicado incluye el ejecutable, dependencias de Qt, plugins requeridos, recursos visuales, script SQL y archivo de configuración de ejemplo.

### Instalación rápida

1. Descargar `AerolineaCPP-v1.0.0-Windows-x64.zip` desde Releases.
2. Extraer completamente el archivo ZIP.
3. Ejecutar `Aerolinea.sql` en Microsoft SQL Server.
4. Copiar `config/database.example.ini` como `config/database.ini`.
5. Configurar la instancia de SQL Server en `database.ini`.
6. Ejecutar `AerolineaCPP.exe`.

> No es necesario instalar Qt Creator para ejecutar el paquete publicado. SQL Server y el controlador ODBC correspondiente sí deben estar disponibles para utilizar la persistencia de datos.

### Integridad del paquete v1.0.0

```text
SHA-256
541317665042110010749f27175bfd1eb7a9622278c8cc411ecf66176099c963
```

---

## 🎓 Información académica original

| Dato | Información |
|------|-------------|
| 🏛️ Institución | Instituto Tecnológico de Las Américas (ITLA) |
| 📚 Asignatura | Estructuras de Datos |
| 🧾 Código | SOF-012 |
| 👨‍🏫 Profesor | Raydelto Hernández Perera |
| 📅 Período | 2018-C1 |
| 👥 Modalidad | Proyecto Final Grupal |

### 👥 Integrantes del proyecto original

| Integrante | Matrícula |
|------------|-----------|
| Francis Jairo Matías Rosario | 2015-2984 |
| Jorge de Jesús Torres Pérez | 2016-3515 |
| Sebastian Donastor Hernández | 2016-3607 |

---

## 🧭 Continuidad académica

**Aerolinea** representa la segunda etapa de una trayectoria académica de tres asignaturas cursadas con el profesor **Raydelto Hernández Perera** en el Instituto Tecnológico de Las Américas (ITLA). La relación entre estos proyectos es **formativa y cronológica**: cada uno responde a una asignatura y objetivo distintos, por lo que no deben interpretarse como dependencias técnicas ni como versiones sucesivas de una misma aplicación.

La secuencia comenzó en **2017-C2** con **Programación II (SOF-004)** y [**Eventix**](https://github.com/Jairo0811/Eventix). Continuó en **2018-C1** con **Estructuras de Datos (SOF-012)**, donde Aerolinea fue desarrollado como proyecto final, y culminó en **2018-C2** con **Programación WEB (SOF-011)** y [**ITLA Crush**](https://github.com/Jairo0811/ITLAcrushReact).

| Orden | Código | Asignatura | Proyecto | Período | Enfoque académico |
|---:|---|---|---|---|---|
| 1 | SOF-004 | Programación II | [**Eventix**](https://github.com/Jairo0811/Eventix) | 2017-C2 | Programación orientada a objetos, lógica de negocio y construcción de una aplicación completa |
| 2 | SOF-012 | Estructuras de Datos | **Aerolinea** | 2018-C1 | Estructuras de datos, listas enlazadas, modelado de relaciones y resolución de rutas |
| 3 | SOF-011 | Programación WEB | [**ITLA Crush**](https://github.com/Jairo0811/ITLAcrushReact) | 2018-C2 | Desarrollo web, JavaScript, React y Firebase |

Vistos en conjunto, los tres proyectos documentan una evolución desde la construcción de aplicaciones orientadas a objetos, pasando por estructuras y algoritmos, hasta el desarrollo web moderno. Aerolinea ocupa el punto intermedio de esa trayectoria al trasladar el aprendizaje hacia el manejo explícito de estructuras de datos y problemas de recorrido y relaciones.

Cada repositorio conserva su identidad académica original y, cuando aplica, incorpora una restauración o modernización posterior orientada a estándares profesionales y portafolio.

---

## 📖 Historia del proyecto

El proyecto nació en **2018-C1** como una aplicación desarrollada en **C++ para consola**, cuyo núcleo académico estaba orientado al uso de estructuras de datos mediante listas enlazadas.

La restauración conserva esa versión dentro de `legacy/`, corrigiendo desperfectos que impedían utilizarla con comodidad en entornos actuales sin convertirla artificialmente en la aplicación moderna.

La versión de 2026 fue construida separadamente en `modern-qt-sqlserver/`. Sobre la idea original incorpora interfaz gráfica, persistencia relacional, separación de responsabilidades, configuración externa y un modelo de distribución reproducible.

El resultado permite comparar directamente dos etapas de aprendizaje y desarrollo separadas por ocho años.

---

## 📈 Evolución: 2018 → 2026

| Característica | Legacy (2018-C1) | AerolineaCPP (2026) |
|:--------------|:----------------:|:-------------------:|
| Aplicación de consola | ✅ | ❌ |
| Interfaz gráfica responsive | ❌ | ✅ Qt 6 Widgets |
| Listas enlazadas | ✅ | Preservadas en Legacy |
| SQL Server | ❌ | ✅ |
| Persistencia de datos | ❌ | ✅ |
| Arquitectura orientada a objetos | ⚠️ Básica | ✅ |
| Separación de responsabilidades | ❌ | ✅ |
| Gestión de aeronaves | ❌ | ✅ |
| Gestión de vuelos | ❌ | ✅ |
| Rutas con escalas | ⚠️ Lógica académica | ✅ |
| Distancia y duración total | ❌ | ✅ |
| Precio total | ❌ | ✅ |
| Configuración externa | ❌ | ✅ |
| Estados visuales de búsqueda | ❌ | ✅ |
| Ventana Acerca de | ❌ | ✅ |
| Identidad visual propia | ❌ | ✅ |
| Build automatizado | ❌ | ✅ GitHub Actions |
| Release portable Windows x64 | ❌ | ✅ |

---

## 🏗️ Arquitectura

```text
                    AerolineaCPP

                Qt 6 Widgets (GUI)
                        │
                        ▼
                  MainWindow
                        │
        ┌───────────────┼───────────────┐
        │                               │
        ▼                               ▼
   RutaManager                 DatabaseManager
        │                               │
        ▼                               ▼
Ruta - Vuelo - Destino - Aeronave    SQL Server
```

La interfaz se encarga de interacción y presentación; `RutaManager` concentra la lógica relacionada con la búsqueda de rutas y `DatabaseManager` encapsula el acceso a SQL Server.

---

## 🛠️ Stack tecnológico

<p align="center">
  <img src="https://skillicons.dev/icons?i=cpp,qt,cmake,git,github" alt="C++, Qt, CMake, Git y GitHub" />
</p>

### Aplicación

- **C++17** — lenguaje principal.
- **Qt 6 Widgets** — interfaz gráfica de escritorio.
- **Qt SQL** — integración de acceso a datos.
- **Qt Resources** — recursos visuales embebidos.
- **Programación Orientada a Objetos** — organización de entidades y responsabilidades.

### Datos

- **Microsoft SQL Server** — persistencia de destinos, rutas, aeronaves y vuelos.
- **ODBC** — conectividad entre Qt y SQL Server.
- **Aerolinea.sql** — creación y carga inicial de la base de datos.

### Ingeniería y distribución

- **CMake** — configuración de compilación.
- **Git / GitHub** — control de versiones y repositorio.
- **GitHub Actions** — build y empaquetado automatizado para Windows.
- **GitHub Releases** — distribución de la versión estable.

---

## 📂 Estructura del repositorio

```text
Aerolinea/
│
├── legacy/
│   └── Proyecto original en C++ para consola (2018-C1)
│
├── modern-qt-sqlserver/
│   ├── Código fuente Qt/C++
│   ├── Aerolinea.sql
│   ├── resources.qrc
│   ├── CMakeLists.txt
│   └── config/
│       └── database.example.ini
│
├── .github/
│   └── workflows/
│       └── release-windows.yml
│
├── README.md
└── .gitignore
```

---

## 🕰️ Legacy

La carpeta `legacy/` conserva el proyecto académico original.

### Características preservadas y restauradas

- 📋 Gestión básica de destinos.
- 🔗 Implementación mediante listas enlazadas.
- 🖥️ Interfaz por consola.
- 📚 Enfoque académico original de Estructuras de Datos.
- 🔧 Correcciones de estabilidad y compatibilidad con compiladores actuales.
- 🚫 Prevención de destinos duplicados.
- 🔤 Búsquedas sin distinción entre mayúsculas y minúsculas.

Esta versión se mantiene deliberadamente separada de AerolineaCPP para conservar el valor histórico del código de 2018.

---

## 🗄️ Base de datos

Las tablas principales son:

- `Destinos`
- `Rutas`
- `Aeronaves`
- `Vuelos`

Script de inicialización:

```text
modern-qt-sqlserver/Aerolinea.sql
```

La configuración local se realiza mediante:

```text
modern-qt-sqlserver/config/database.ini
```

El repositorio incluye únicamente `database.example.ini`; las credenciales locales no deben versionarse.

---

## 🧪 Build y distribución

El proyecto utiliza **CMake** y dispone de automatización en **GitHub Actions** para construir la versión Windows y preparar el paquete portable.

Esto permite generar una distribución reproducible sin depender de que el desarrollador tenga Qt instalado en la máquina desde la que se publica el Release.

---

## 🚦 Estado del proyecto

### ✅ AerolineaCPP v1.0.0 — Finalizado

- [x] Preservación del proyecto original.
- [x] Restauración y estabilización de Legacy.
- [x] Modernización con C++17 y Qt 6.
- [x] Integración con SQL Server.
- [x] Gestión de destinos, rutas, aeronaves y vuelos.
- [x] Búsqueda de rutas directas y con escalas.
- [x] Cálculo de distancia, duración y precio.
- [x] Interfaz responsive en tema oscuro.
- [x] Estados visuales y acción Limpiar.
- [x] Identidad visual de AerolineaCPP.
- [x] Créditos académicos e identidad institucional.
- [x] Configuración portable de base de datos.
- [x] Build automatizado para Windows.
- [x] Release v1.0.0 publicado.
- [x] SHA-256 del paquete publicado.
- [x] Documentación para portafolio.

---

## 📅 Línea de tiempo

| Fecha | Evento |
|--------|--------|
| 🎓 **2018-C1** | Desarrollo del proyecto original para Estructuras de Datos (SOF-012) |
| 🔧 **2026** | Restauración y estabilización del código Legacy |
| 🚀 **Junio 2026** | Modernización con C++17, Qt 6, SQL Server y CMake |
| 🎨 **2026** | Rediseño responsive, identidad visual y mejoras de UX |
| ⚙️ **2026** | Automatización de build y distribución con GitHub Actions |
| 📦 **2026** | Publicación de AerolineaCPP v1.0.0 para Windows x64 |
