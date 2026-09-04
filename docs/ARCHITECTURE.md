# Arquitectura de AerolineaCPP v1.1.0

## Propósito

AerolineaCPP es una aplicación de escritorio de **consulta y optimización de rutas**. Conserva el objetivo académico de Estructuras de Datos y lo amplía con persistencia, GUI, seguridad, pruebas y automatización sin convertir el repositorio en un sistema aeronáutico empresarial.

## Vista de alto nivel

```text
Usuario
  │
  ▼
MainWindow (Qt Widgets)
  │
  ├──────────────► DatabaseManager
  │                    │
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
- representación textual de resultados y estados.

No contiene consultas SQL ni implementa el algoritmo del grafo.

### DatabaseManager

Encapsula el acceso a SQL Server mediante Qt SQL/ODBC.

Responsabilidades:

- construir una cadena ODBC validada;
- aplicar TLS y restricciones del driver;
- obtener configuración no secreta;
- leer credenciales SQL únicamente desde variables de entorno;
- consultar destinos, rutas, aeronaves y vuelos;
- validar límites de los datos recibidos;
- trabajar con intención de solo lectura.

La aplicación no necesita permisos `INSERT`, `UPDATE`, `DELETE`, `ALTER` ni `CONTROL` durante la operación normal.

### RutaManager

Es el núcleo algorítmico.

Construye el grafo a partir de `QVector<Ruta>` y calcula el camino óptimo según `CriterioRuta`:

```text
MenosEscalas    peso = 1 por arista
MenorDistancia  peso = DistanciaKm
MenorDuracion   peso = DuracionMinutos
MenorPrecio     peso = Precio del Vuelo
```

El criterio `MenorPrecio` descarta un tramo cuando no existe un vuelo con precio asociado, ya que no es posible comparar de forma válida su costo monetario.

Dijkstra mantiene para cada destino el mejor costo conocido y la arista predecesora. Al alcanzar el destino reconstruye el camino desde los predecesores.

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

`tests/rutamanager_tests.cpp` cubre el comportamiento algorítmico sin necesitar una GUI ni SQL Server.

El pipeline usa dos capas:

1. **Unit tests**: Qt Test + CTest sobre Windows.
2. **Database contract**: SQL Server 2022 real sobre Linux, incluyendo bootstrap doble e invariantes del esquema/seed.

## Distribución

GitHub Actions produce un ZIP portable para Windows x64 mediante `windeployqt`, genera SHA-256 y separa el job de compilación del job con permiso de publicación.

Una release manual solo puede publicarse si build, unit tests y database contract finalizan correctamente.
