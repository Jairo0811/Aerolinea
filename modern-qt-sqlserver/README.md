# AerolineaCPP v1.1.1

**Final Portfolio Maintenance Release** de la modernización de Aerolinea (proyecto académico ITLA, Estructuras de Datos SOF-012, 2018-C1).

## Objetivo

Aplicación Qt 6/C++17 de consulta y optimización de rutas aéreas almacenadas en Microsoft SQL Server. El motor permite seleccionar el recorrido con:

- menos escalas;
- menor distancia;
- menor duración;
- menor precio.

Todos los criterios se resuelven con Dijkstra sobre el grafo dirigido de rutas.

## Build local

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Requiere CMake 3.19+ y Qt 6.5+ (Core, Widgets, Sql, Test). La versión del producto se define una sola vez en `project(AerolineaCPP VERSION ...)` y de ahí salen el título, los metadatos del `.exe` y el instalador.

## Base de datos

1. Ejecuta `Aerolinea.sql` en SQL Server.
2. Configura `database.ini` (ver abajo).
3. Mantén `encrypt=true` y `trustServerCertificate=false` fuera de entornos locales controlados.
4. Prefiere autenticación integrada.
5. Para autenticación SQL utiliza `trustedConnection=false` con `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`.
6. Asigna al principal de la aplicación únicamente el rol `AerolineaReader`.

`Aerolinea.sql` es idempotente y CI comprueba que pueda ejecutarse dos veces sobre SQL Server 2022 sin duplicar el seed.

## Configuración

Orden de búsqueda de `database.ini`:

1. **Portable:** `config/database.ini` junto a `AerolineaCPP.exe`, si existe.
2. **Usuario (modo instalado):** `%LOCALAPPDATA%\JM Software Solutions\AerolineaCPP\database.ini` (`QStandardPaths::AppConfigLocation`). Se crea una vez a partir de la plantilla embebida y nunca se sobrescribe.
3. **Predeterminado:** valores seguros internos.

`config/database.example.ini` es solo la plantilla. Desde la aplicación: **Base de datos → Abrir carpeta de configuración** y, tras editar, **Reintentar conexión** (`F5`).

## Seguridad

La aplicación es de escritorio y no expone endpoints HTTP. El límite de confianza está en la sesión local, el archivo de configuración no secreto, las variables de entorno y SQL Server.

## Estado

**Portfolio Complete / Frozen.** No es un sistema de reservas ni de operación real de aerolíneas.
