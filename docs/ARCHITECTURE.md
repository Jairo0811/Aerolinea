# Arquitectura de AerolineaCPP v1.1.1

## Propósito

AerolineaCPP es una aplicación de escritorio de **consulta y optimización de rutas**. Conserva el objetivo académico de Estructuras de Datos y lo amplía con persistencia, GUI, seguridad, pruebas y automatización sin convertir el repositorio en un sistema aeronáutico empresarial.

## Vista de alto nivel

```text
Usuario
  │
  ▼
MainWindow (Qt Widgets)
  │
  ├──────────────► DatabaseManager ──► ConfiguracionBaseDatos
  │                    │                   (database.ini)
  │                    ▼
  │               Qt SQL / QODBC
  │                    │
  │                    ▼
  │               SQL Server 2022
  │
  ▼
RutaManager
  │
  ├── grafo dirigido de Ruta
  ├── catálogo de Vuelo
  ├── catálogo de Aeronave
  └── Dijkstra multicriterio
         │
         ▼
    QVector<Ruta>
```

## Responsabilidades

### MainWindow

Responsable de presentación e interacción:

- origen y destino;
- criterio de optimización;
- indicadores de datos cargados;
- ejecución de búsquedas;
- representación textual de resultados y estados;
- carga y recarga de datos (`cargarDatosDesdeBaseDeDatos()`), compartida por el arranque y por **Reintentar conexión** (`F5`);
- acceso a la carpeta de configuración mediante `QDesktopServices`.

No contiene consultas SQL ni implementa el algoritmo del grafo. Si la pantalla es pequeña o usa escalado alto, el contenido se desplaza dentro de un `QScrollArea` en lugar de recortarse.

### DatabaseManager

Encapsula el acceso a SQL Server mediante Qt SQL/ODBC.

Responsabilidades:

- construir una cadena ODBC validada;
- aplicar TLS y restricciones del driver;
- obtener configuración no secreta a través de `ConfiguracionBaseDatos`;
- reconectar releyendo la configuración vigente (`reconectar()`);
- leer credenciales SQL únicamente desde variables de entorno;
- consultar destinos, rutas, aeronaves y vuelos;
- validar límites de los datos recibidos;
- trabajar con intención de solo lectura.

La aplicación no necesita permisos `INSERT`, `UPDATE`, `DELETE`, `ALTER` ni `CONTROL` durante la operación normal.

### ConfiguracionBaseDatos

Resuelve qué `database.ini` usar. No depende de GUI ni de SQL, por lo que tiene pruebas propias.

```text
Portable
applicationDir/config/database.ini          ← prioridad 1, si existe

Installed
QStandardPaths::AppConfigLocation
  → %LOCALAPPDATA%/JM Software Solutions/AerolineaCPP/database.ini
                                            ← prioridad 2; se crea una vez
                                              desde :/config/database.example.ini
                                              (QIODevice::NewOnly: nunca sobrescribe)

Predeterminado
valores seguros internos                    ← prioridad 3
```

`database.example.ini` es solo plantilla: se embebe en el ejecutable y nunca se lee como configuración del usuario.

### RutaManager

Es el núcleo algorítmico.

Construye el grafo a partir de `QVector<Ruta>` y calcula el camino óptimo según `CriterioRuta`:

```text
MenosEscalas    peso = 1 por arista
MenorDistancia  peso = DistanciaKm
MenorDuracion   peso = DuracionMinutos
MenorPrecio     peso = Precio del Vuelo
```

El criterio `MenorPrecio` usa, para cada ruta, el vuelo más barato (empate: menor Id) y descarta un tramo cuando no existe un vuelo con precio válido, ya que no es posible comparar de forma válida su costo monetario. Pesos negativos o no finitos nunca entran al algoritmo.

Dijkstra mantiene para cada destino el mejor costo conocido y la arista predecesora. Al alcanzar el destino reconstruye el camino desde los predecesores. Los nodos se identifican sin distinguir mayúsculas/minúsculas y los empates de costo se resuelven por orden de descubrimiento, de modo que el resultado es determinista en cualquier compilador.

### Entidades

- `Destino`: nodo del grafo.
- `Ruta`: arista dirigida con distancia y duración.
- `Vuelo`: dato operacional asociado a una ruta, incluyendo precio.
- `Aeronave`: información del equipo asociado al vuelo.

## Persistencia

`Aerolinea.sql` crea y configura:

- `Destinos`;
- `Rutas`;
- `Aeronaves`;
- `Vuelos`;
- constraints de dominio;
- índices únicos;
- claves foráneas;
- datos seed;
- rol `AerolineaReader`.

El script es transaccional e idempotente. GitHub Actions lo ejecuta dos veces sobre SQL Server 2022 para verificar el contrato de instalación.

## Seguridad

```text
config/database.ini
   │
   ├── driver / servidor / base / flags TLS
   └── NO contiene contraseñas

AEROLINEA_DB_USER
AEROLINEA_DB_PASSWORD
   │
   ▼
DatabaseManager
   │
   ▼
ODBC + TLS
   │
   ▼
SQL Server
   │
   ▼
AerolineaReader (SELECT only)
```

Controles adicionales:

- whitelist de ODBC Driver 17/18;
- escape/validación de atributos de conexión;
- timeout;
- límites de filas;
- validación de identificadores, texto y valores numéricos;
- queries estáticas o parametrizadas;
- mitigaciones de compilador/linker.

## Pruebas

- `tests/rutamanager_tests.cpp` (15 casos) cubre el comportamiento algorítmico sin GUI ni SQL Server.
- `tests/configuracion_tests.cpp` (5 casos) cubre la resolución portable/usuario con `QStandardPaths::setTestModeEnabled(true)` y directorios temporales.

El pipeline usa tres capas:

1. **Unit tests**: Qt Test + CTest sobre Windows.
2. **Database contract**: SQL Server 2022 real sobre Linux, incluyendo bootstrap doble, claves foráneas, seed y permisos SELECT-only.
3. **Installer validation**: instalación silenciosa en `Program Files`, arranque de la app instalada, creación de la configuración de usuario y desinstalación silenciosa conservando esa configuración.

## Distribución

Un único workflow (`.github/workflows/release-windows.yml`) produce, desde el mismo commit:

```text
project(AerolineaCPP VERSION X.Y.Z)
        │
        ├── config.hpp            → título y "Acerca de"
        ├── app_icon.rc.in        → icono + VERSIONINFO del .exe
        └── ISCC /DMyAppVersion   → Setup.exe

AerolineaCPP-vX.Y.Z-Windows-x64.zip   + .sha256   (windeployqt)
AerolineaCPP-vX.Y.Z-Setup.exe         + .sha256   (Inno Setup)
```

Los logos, el icono y la plantilla de configuración están embebidos con `qt_add_resources`; no se distribuyen copias sueltas.

Una release manual (`workflow_dispatch`) solo se publica si build, unit tests, database contract e installer validation finalizan correctamente, si la versión coincide con CMake y si el tag no existe. El job de publicación es el único con `contents: write` y no compila código.
