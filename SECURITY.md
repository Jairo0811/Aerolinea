# Security Policy

## Estado soportado

| Versión | Soporte |
|---|---|
| 1.1.x (última: 1.1.1) | ✅ Correcciones de seguridad y bugs reales (proyecto congelado) |
| 1.0.x | ⚠️ Histórica; actualizar a 1.1.x |
| Legacy 2018 | ⚠️ Solo preservación académica |

## Modelo de seguridad

AerolineaCPP es una aplicación de escritorio. No expone una API HTTP ni servicios de red propios. Sus límites de confianza principales son:

- la sesión local del sistema operativo;
- `database.ini`, que solo debe contener configuración no secreta (ver ubicación abajo);
- variables de entorno para credenciales SQL cuando se use autenticación SQL;
- el canal ODBC/TLS hacia Microsoft SQL Server;
- el principal de base de datos limitado al rol `AerolineaReader`.

## Ubicación de la configuración

| Prioridad | Modo | Archivo |
|---:|---|---|
| 1 | Portable | `config\database.ini` junto a `AerolineaCPP.exe` |
| 2 | Instalado / usuario | `%LOCALAPPDATA%\JM Software Solutions\AerolineaCPP\database.ini` |
| 3 | Predeterminado | valores internos seguros (TLS y autenticación integrada) |

- En modo instalado la configuración vive en el perfil del usuario: no se escribe en `Program Files` ni requiere permisos de administrador.
- La copia inicial se crea desde la plantilla embebida en el ejecutable y nunca se sobrescribe un archivo existente.
- El desinstalador conserva ese archivo; no contiene secretos.
- `database.ini` **no contiene contraseñas**. Las credenciales SQL provienen exclusivamente de `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`.
- **TLS es obligatorio:** `encrypt=false` hace que la conexión se rechace antes de intentarla.
- El rol de la aplicación, `AerolineaReader`, solo tiene `SELECT` sobre las cuatro tablas, y la conexión usa `ApplicationIntent=ReadOnly`.
- Los mensajes de la interfaz y los logs nunca incluyen usuario, contraseña ni la cadena de conexión; solo la ruta del archivo de configuración y el código del controlador ODBC.

## Recomendaciones de despliegue

- Mantén `encrypt=true`.
- Mantén `trustServerCertificate=false` en producción.
- Prefiere autenticación integrada de Windows cuando sea viable.
- No almacenes contraseñas en `database.ini`, scripts, commits ni logs.
- Usa `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD` únicamente cuando necesites autenticación SQL.
- Asigna el principal de la aplicación a `AerolineaReader`; evita roles administrativos.
- Instala Microsoft ODBC Driver 17 u 18 desde una fuente oficial.
- Verifica el archivo `.sha256` distribuido junto a cada release nueva.
- Los binarios no tienen firma Authenticode comercial; Windows SmartScreen puede advertir sobre ellos. Descárgalos solo desde GitHub Releases de este repositorio y verifica el SHA-256.

## CI

El contrato de base de datos de GitHub Actions usa un contenedor SQL Server efímero con una contraseña `sa` aleatoria generada en cada ejecución. No hay contraseñas fijas en el repositorio y el valor no sirve para ninguna instalación real.

## Reporte de vulnerabilidades

No publiques secretos, credenciales, exploits funcionales ni datos sensibles en un issue público.

Utiliza preferentemente **GitHub → Security → Report a vulnerability / Security Advisories** cuando esa opción esté disponible para el repositorio. Si necesitas coordinar primero el canal de reporte, abre un issue público sin detalles sensibles solicitando un medio privado de contacto.

Incluye:

- versión afectada;
- componente afectado;
- pasos mínimos para reproducir;
- impacto esperado;
- mitigación conocida, si existe.

## Alcance

Se consideran dentro de alcance defectos en:

- validación de configuración ODBC;
- exposición de credenciales;
- cifrado de conexión;
- consultas SQL;
- permisos de base de datos;
- empaquetado/release;
- corrupción o ejecución inesperada causada por entradas controlables.

El código dentro de `legacy/` se conserva con fines académicos. Las correcciones allí priorizan eliminar riesgos evidentes sin reescribir su naturaleza histórica.
