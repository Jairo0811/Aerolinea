# AerolineaCPP v1.1.0

**Final Portfolio Edition** de la modernización de Aerolinea.

## Objetivo

Aplicación Qt 6/C++17 de consulta y optimización de rutas aéreas almacenadas en Microsoft SQL Server. El motor permite seleccionar el recorrido con:

- menos escalas;
- menor distancia;
- menor duración;
- menor precio.

Los criterios ponderados utilizan Dijkstra.

## Build local

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## Base de datos

1. Ejecuta `Aerolinea.sql` en SQL Server.
2. Copia `config/database.example.ini` a `config/database.ini`.
3. Mantén `encrypt=true` y `trustServerCertificate=false` fuera de entornos locales controlados.
4. Prefiere autenticación integrada.
5. Para autenticación SQL utiliza `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`.
6. Asigna al principal de la aplicación únicamente el rol `AerolineaReader`.

`Aerolinea.sql` es idempotente y CI comprueba que pueda ejecutarse dos veces sobre SQL Server 2022 sin duplicar el seed.

## Seguridad

La aplicación es de escritorio y no expone endpoints HTTP. El límite de confianza está en la sesión local, el archivo de configuración no secreto, las variables de entorno y SQL Server.

## Estado

**Portfolio Complete / Maintenance.** No es un sistema de reservas ni de operación real de aerolíneas.
