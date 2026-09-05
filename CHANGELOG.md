# Changelog

Todos los cambios relevantes de AerolineaCPP se documentan aquí.

## [1.1.0] - 2026-09-04

### Added

- Selector de criterio de optimización en la interfaz.
- Motor multicriterio basado en Dijkstra.
- Optimización por menos escalas, menor distancia, menor duración y menor precio.
- API `calcularRuta()` separada de la presentación textual para facilitar pruebas.
- 7 pruebas unitarias de `RutaManager` con Qt Test/CTest.
- Contrato de integración contra SQL Server 2022 en GitHub Actions.
- `SECURITY.md` y política de mantenimiento.
- Licencia de uso de portafolio que respeta la autoría grupal del proyecto original.
- Instalador profesional `AerolineaCPP-v1.1.0-Setup.exe` generado con Inno Setup.
- Accesos directos de menú Inicio y escritorio opcional, registro en aplicaciones instaladas y desinstalador.
- Validación automática de instalación y desinstalación silenciosa antes de publicar el instalador.
- Checksum SHA-256 independiente para el `Setup.exe`.

### Changed

- Estado del proyecto a **Portfolio Complete / Maintenance**.
- Versión CMake a 1.1.0.
- Interfaz y textos actualizados a **Final Portfolio Edition**.
- README reescrito para describir el alcance real como consulta/optimización y no CRUD.
- Pipeline de release condicionado a build, unit tests y contrato de base de datos.
- Distribución Windows disponible tanto como ZIP portable como instalador Inno Setup.

### Security

- Integrado el hardening del PR #3.
- SQL Server con cifrado y `trustServerCertificate=false` por defecto.
- Credenciales SQL movidas a variables de entorno.
- Whitelist de ODBC Driver 17/18.
- Validación de configuración y datos recibidos.
- Script SQL transaccional, idempotente, con constraints, índices, claves foráneas y rol `AerolineaReader`.
- Build y publicación separados por permisos mínimos.
- SHA-256 generado automáticamente para paquetes nuevos.

## [1.0.0] - 2026-08-10

### Added

- Primera release estable de la modernización Qt/C++17.
- Interfaz Qt 6 Widgets.
- Persistencia SQL Server mediante ODBC.
- Búsqueda de rutas con escalas.
- Consulta de vuelos y aeronaves.
- Cálculo de distancia, duración y precio.
- CMake y distribución portable para Windows x64.
- Preservación de la versión Legacy 2018-C1.
