<p align="center">
  <img src="modern-qt-sqlserver/aerolineacpp_logo.png" width="380" alt="AerolineaCPP">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Estado-Portfolio%20Complete-22C55E?style=for-the-badge" alt="Portfolio Complete">
  <img src="https://img.shields.io/badge/Versión-1.1.0-0284C7?style=for-the-badge" alt="v1.1.0">
  <img src="https://img.shields.io/badge/ITLA-2018--C1-0057B8?style=for-the-badge" alt="ITLA 2018-C1">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white" alt="C++17">
  <img src="https://img.shields.io/badge/Qt-6-41CD52?style=flat-square&logo=qt&logoColor=white" alt="Qt 6">
  <img src="https://img.shields.io/badge/SQL%20Server-2022-CC2927?style=flat-square&logo=microsoftsqlserver&logoColor=white" alt="SQL Server 2022">
  <img src="https://img.shields.io/badge/CMake-Build-064F8C?style=flat-square&logo=cmake&logoColor=white" alt="CMake">
</p>

# Aerolinea / AerolineaCPP

**Aerolinea** nació como proyecto final de **Estructuras de Datos (SOF-012)** en el **Instituto Tecnológico de Las Américas (ITLA)** durante **2018-C1**. La versión original de consola utilizaba listas enlazadas para modelar destinos y relaciones.

En 2026 el proyecto fue recuperado sin borrar su historia. El repositorio conserva dos etapas deliberadamente separadas:

- `legacy/`: código académico original restaurado para entornos actuales.
- `modern-qt-sqlserver/`: **AerolineaCPP**, reinterpretación de escritorio con C++17, Qt 6, SQL Server, CMake, seguridad reforzada, pruebas y CI/CD.

La edición **v1.1.0 — Final Portfolio Edition** cierra la modernización como pieza de portafolio. A partir de esta versión el estado objetivo es **Maintenance**: correcciones, compatibilidad y seguridad, no expansión hacia un sistema comercial de operación aérea.

## Qué demuestra este proyecto

Este repositorio está diseñado para mostrar evolución técnica a través del tiempo:

```text
2018-C1
C++ de consola
   ↓
Listas enlazadas y estructuras de datos
   ↓
2026
C++17 + Qt 6 Widgets
   ↓
SQL Server + ODBC + CMake
   ↓
Dijkstra multicriterio + pruebas + CI/CD + hardening
```

## AerolineaCPP v1.1.0

La aplicación consulta un grafo de rutas almacenado en SQL Server y permite calcular un recorrido óptimo según cuatro criterios:

| Criterio | Estrategia |
|---|---|
| **Menos escalas** | costo uniforme por tramo |
| **Menor distancia** | peso por kilómetros |
| **Menor duración** | peso por minutos |
| **Menor precio** | peso por precio del vuelo |

Para criterios ponderados se utiliza **Dijkstra**. El resultado incluye los tramos, vuelo, aerolínea, aeronave, capacidad, escalas, distancia total, duración total y precio total cuando existe información de precio para todo el recorrido.

### Funcionalidades

- Consulta de destinos desde SQL Server.
- Consulta de rutas, vuelos y aeronaves.
- Búsqueda de rutas directas y con escalas.
- Optimización multicriterio.
- Cálculo de distancia, duración y precio agregados.
- Resumen de destinos, rutas, aeronaves y vuelos registrados.
- Interfaz Qt 6 en tema oscuro y diseño adaptable de escritorio.
- Configuración externa mediante `database.ini`.
- Recursos visuales embebidos.
- Ventana **Acerca de** con contexto académico y créditos.
- Paquete portable para Windows x64 generado por GitHub Actions.
- Instalador profesional de Windows generado con Inno Setup.

> **Alcance correcto:** AerolineaCPP es una aplicación de **consulta y optimización de rutas**. No ofrece CRUD administrativo de datos desde la GUI y no se presenta como un sistema de reservas u operación real de aerolíneas.

## Seguridad

La edición final incorpora límites de confianza explícitos:

- cifrado de conexión SQL mediante TLS;
- `trustServerCertificate=false` por defecto;
- autenticación integrada como opción preferida;
- usuario y contraseña SQL mediante `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`;
- `database.ini` excluido del control de versiones;
- controlador restringido a Microsoft ODBC Driver 17/18;
- timeout e intención de lectura;
- validación de valores procedentes de SQL Server;
- consultas estáticas o parametrizadas;
- constraints, índices únicos y claves foráneas en la base de datos;
- rol de mínimo privilegio `AerolineaReader`;
- mitigaciones del compilador y linker;
- pipeline de build con permisos de solo lectura y publicación aislada.

Consulta [SECURITY.md](SECURITY.md) para el modelo de seguridad y reporte de vulnerabilidades.

## Pruebas y CI

La v1.1.0 incorpora pruebas automatizadas para `RutaManager`:

- menos escalas;
- menor distancia;
- menor duración;
- menor precio;
- destino inalcanzable;
- origen y destino iguales;
- búsquedas sin distinguir mayúsculas/minúsculas.

GitHub Actions ejecuta además un **contrato real contra SQL Server 2022** que levanta `Aerolinea.sql`, lo ejecuta dos veces para comprobar idempotencia y valida tablas, rol mínimo y datos seed.

La distribución de Windows también recompila la aplicación, ejecuta las pruebas, genera el instalador con Inno Setup y realiza una **instalación/desinstalación silenciosa de validación** antes de publicar el `Setup.exe` en GitHub Releases.

## Instalación

### Requisitos

- Windows 10/11 x64.
- Microsoft SQL Server.
- Microsoft ODBC Driver 17 u 18 for SQL Server.
- TLS correctamente configurado para el servidor de base de datos.

### Instalador recomendado

1. Descarga `AerolineaCPP-v1.1.0-Setup.exe` desde **GitHub Releases**.
2. Ejecuta el instalador y acepta los permisos de administrador para instalar en `Program Files`.
3. El asistente registra **AerolineaCPP** en el menú Inicio y permite crear opcionalmente un acceso directo en el escritorio.
4. Ejecuta `Aerolinea.sql` con permisos para crear/configurar `AerolineaDB`.
5. Copia `config/database.example.ini` como `config/database.ini` y configura la instancia.
6. Para autenticación SQL define `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`.
7. Asigna al principal de la aplicación únicamente el rol `AerolineaReader`.
8. Inicia AerolineaCPP desde el menú Inicio o el acceso directo.

El instalador incluye desinstalador y aparece en las aplicaciones instaladas de Windows. La configuración creada manualmente por el usuario no se elimina de forma forzada por el script de instalación.

> **Firma digital:** el instalador de portafolio no está firmado con un certificado Authenticode comercial. Dependiendo de la reputación del archivo, Windows SmartScreen puede mostrar una advertencia antes de ejecutarlo.

### Paquete portable

Como alternativa al instalador:

1. Descarga `AerolineaCPP-v1.1.0-Windows-x64.zip` desde **GitHub Releases**.
2. Extrae completamente el ZIP.
3. Ejecuta `Aerolinea.sql` con permisos para crear/configurar `AerolineaDB`.
4. Copia `config/database.example.ini` como `config/database.ini`.
5. Configura servidor, base de datos y autenticación.
6. Para autenticación SQL define las variables `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`.
7. Asigna al principal de la aplicación únicamente el rol `AerolineaReader`.
8. Ejecuta `AerolineaCPP.exe`.

Las distribuciones publicadas incluyen archivos `.sha256` para verificar integridad.

## Arquitectura

```text
                 AerolineaCPP

              Qt 6 Widgets GUI
                     │
                     ▼
                 MainWindow
                     │
          ┌──────────┴──────────┐
          ▼                     ▼
     RutaManager          DatabaseManager
          │                     │
          ▼                     ▼
  Dijkstra / grafo        Qt SQL / ODBC
          │                     │
          └──────────┬──────────┘
                     ▼
        Ruta · Vuelo · Destino · Aeronave
                     │
                     ▼
              SQL Server 2022
```

Más detalle en [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Stack tecnológico

### Aplicación

- C++17
- Qt 6 Core / Widgets / SQL
- Programación orientada a objetos
- Algoritmos de grafos y Dijkstra

### Datos

- Microsoft SQL Server 2022
- ODBC
- Script SQL idempotente con constraints, índices y rol de solo lectura

### Ingeniería y distribución

- CMake
- Qt Test
- CTest
- Inno Setup
- Git / GitHub
- GitHub Actions
- GitHub Releases

## Estructura

```text
Aerolinea/
├── .github/workflows/
│   ├── release-windows.yml
│   ├── publish-v1.1.0.yml
│   └── publish-installer-v1.1.0.yml
├── docs/
│   └── ARCHITECTURE.md
├── installer/
│   └── aerolineacpp.iss
├── legacy/
│   └── Proyecto académico restaurado de 2018-C1
├── modern-qt-sqlserver/
│   ├── tests/
│   │   └── rutamanager_tests.cpp
│   ├── config/
│   │   └── database.example.ini
│   ├── Aerolinea.sql
│   ├── CMakeLists.txt
│   └── Código fuente Qt/C++
├── CHANGELOG.md
├── LICENSE
├── SECURITY.md
└── README.md
```

## Origen académico

| Dato | Información |
|---|---|
| Institución | Instituto Tecnológico de Las Américas (ITLA) |
| Asignatura | Estructuras de Datos |
| Código | SOF-012 |
| Profesor | Raydelto Hernández Perera |
| Período | 2018-C1 |
| Modalidad | Proyecto final grupal |

### Integrantes del proyecto original

- Francis Jairo Matías Rosario — 2015-2984
- Jorge de Jesús Torres Pérez — 2016-3515
- Sebastian Donastor Hernández — 2016-3607

La modernización de 2026 preserva explícitamente esos créditos y no convierte retroactivamente el trabajo grupal original en una obra individual.

## Continuidad académica

Aerolinea forma parte de una secuencia formativa cursada con el profesor Raydelto Hernández Perera:

1. **Eventix** — Programación II (SOF-004), 2017-C2.
2. **Aerolinea** — Estructuras de Datos (SOF-012), 2018-C1.
3. **ITLA Crush** — Programación WEB (SOF-011), 2018-C2.

La relación es académica y cronológica; los repositorios no son dependencias técnicas entre sí.

## Lo que AerolineaCPP no intenta ser

No es un Passenger Service System (PSS), GDS, motor de reservas ni sistema certificado para operación aérea. No implementa, entre otras cosas:

- reservas y pasajeros;
- inventario real de asientos;
- emisión de tickets;
- pagos;
- check-in;
- horarios operacionales;
- gestión de tripulación;
- NDC / Offers & Orders;
- integraciones con aerolíneas reales.

Ese límite es intencional. El valor del proyecto está en **estructuras de datos, algoritmos, modernización de legado, seguridad, persistencia y prácticas de ingeniería**, no en competir con plataformas aeronáuticas empresariales.

## Estado del proyecto

### Portfolio Complete / Maintenance

La v1.1.0 constituye el cierre funcional previsto.

Se aceptan como evolución futura:

- correcciones de bugs;
- actualizaciones de dependencias;
- hardening adicional;
- compatibilidad con nuevas versiones de Qt, CMake, ODBC o SQL Server;
- mejoras menores de accesibilidad y UX.

Nuevos módulos empresariales quedan fuera del alcance de este repositorio.

## Licencia y uso

El código se publica para revisión académica y de portafolio. El proyecto original fue grupal y conserva múltiples titulares de derechos. Consulta [LICENSE](LICENSE) antes de copiar, redistribuir, modificar o utilizar comercialmente el código.
