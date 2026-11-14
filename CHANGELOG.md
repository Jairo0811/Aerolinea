# Changelog

Todos los cambios relevantes de AerolineaCPP se documentan aquí.

## [1.1.1] - 2026-11-14

**Final Maintenance / Packaging & Quality Release.** Versión de mantenimiento que cierra la línea 1.x: corrige defectos reales de configuración, empaquetado, algoritmo e interfaz sin añadir funcionalidades. Los assets se compilan y verifican desde el mismo commit al que apunta el tag.

### Fixed

- **Configuración en modo instalado:** la aplicación ya no exige editar archivos en `Program Files`. Usa `database.ini` en la carpeta de configuración del usuario (`QStandardPaths::AppConfigLocation`), creado una única vez desde la plantilla embebida y nunca sobrescrito. `config/database.ini` junto al ejecutable sigue teniendo prioridad (modo portable).
- `database.example.ini` deja de leerse como si fuera la configuración del usuario; ahora es solo plantilla.
- **Icono global:** `main.cpp` apuntaba a un recurso inexistente (`:/aerolineacpp_logo.png`); ahora usa el `.ico` embebido.
- **Menor precio:** se usa el vuelo más barato de cada ruta (empate: menor Id), no el primero recibido.
- Precios negativos o no finitos se descartan antes de Dijkstra.
- Rutas con distinta capitalización del mismo destino forman un único nodo; antes la reconstrucción del camino fallaba.
- Empates de costo resueltos de forma estable e independiente de la implementación de `std::priority_queue`.
- La interfaz decide "ruta encontrada" con el resultado de `calcularRuta()` en lugar de buscar texto en el mensaje.
- Origen y destino ya no coinciden al cargar los datos.
- La ventana declaraba 960×690 de mínimo pero su contenido necesitaba ~780 px de alto: en 1366×768 o 1080p al 150 % se recortaba. El contenido ahora se desplaza y la ventana inicia maximizada si no cabe.
- Título de ventana duplicado y bordes anidados en **Acerca de**; el logo ITLA tiene placa clara para contraste.
- El logo de cabecera se veía diminuto por el relleno transparente del PNG (1024×1536); se recortó al contenido (2,1 MB → 188 KB).

### Added

- **Reintentar conexión** (botón y menú **Base de datos**, `F5`): vuelve a leer `database.ini` y reconecta sin reiniciar la aplicación.
- **Abrir carpeta de configuración** desde el menú **Base de datos**.
- Metadatos de Windows en `AerolineaCPP.exe` (Propiedades → Detalles) generados por CMake desde `project(VERSION)`, con atribución al proyecto académico grupal de 2018 y a la modernización de 2026.
- 13 pruebas nuevas (20 en total): resolución de configuración portable/usuario, creación segura sin sobrescritura, y casos de Dijkstra (vuelo más barato, precios inválidos, capitalización, ciclos, empates, totales).

### Changed

- **CI/CD consolidado** en un único workflow genérico (`release-windows.yml`): en cada PR compila, prueba, valida el contrato SQL Server, genera ZIP y `Setup.exe` y ejecuta instalación/desinstalación silenciosa; la publicación manual exige que la versión coincida con CMake, no reescribe tags existentes y publica los 4 assets con SHA-256.
- Instalador Inno Setup genérico: la versión llega desde CMake, incluye `LICENSE.txt` y un `LEEME.txt` actualizado, y el desinstalador conserva la configuración personal.
- El contrato SQL de CI usa una contraseña aleatoria por ejecución para el contenedor efímero (sin valores fijos en el repositorio) y verifica también claves foráneas y permisos SELECT-only de `AerolineaReader`.
- Recursos Qt con una sola fuente de verdad: todo embebido con `qt_add_resources`, sin copias físicas de logos ni fallback a disco.

### Removed

- Workflows puntuales `publish-v1.1.0.yml` y `publish-installer-v1.1.0.yml` (integrados en el pipeline general).
- `resources.qrc` (CMake no lo usaba), `app_icon.rc` (sustituido por `app_icon.rc.in`), `AerolineaCPP_es_DO.ts` (vacío, sin uso), `aerolineacpp_logo.png.png` (duplicado binario exacto), `aerolineacpp_logo.jpeg` y `Arolinea-CPP.png` (variantes del logo sin referencias). Siguen disponibles en el historial de Git.

**Status: Portfolio Complete / Frozen**

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
