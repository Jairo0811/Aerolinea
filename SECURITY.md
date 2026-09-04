# Security Policy

## Estado soportado

| Versión | Soporte |
|---|---|
| 1.1.x | ✅ Mantenimiento de seguridad |
| 1.0.x | ⚠️ Histórica; actualizar a 1.1.x |
| Legacy 2018 | ⚠️ Solo preservación académica |

## Modelo de seguridad

AerolineaCPP es una aplicación de escritorio. No expone una API HTTP ni servicios de red propios. Sus límites de confianza principales son:

- la sesión local del sistema operativo;
- `config/database.ini`, que solo debe contener configuración no secreta;
- variables de entorno para credenciales SQL cuando se use autenticación SQL;
- el canal ODBC/TLS hacia Microsoft SQL Server;
- el principal de base de datos limitado al rol `AerolineaReader`.

## Recomendaciones de despliegue

- Mantén `encrypt=true`.
- Mantén `trustServerCertificate=false` en producción.
- Prefiere autenticación integrada de Windows cuando sea viable.
- No almacenes contraseñas en `database.ini`, scripts, commits ni logs.
- Usa `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD` únicamente cuando necesites autenticación SQL.
- Asigna el principal de la aplicación a `AerolineaReader`; evita roles administrativos.
- Instala Microsoft ODBC Driver 17 u 18 desde una fuente oficial.
- Verifica el archivo `.sha256` distribuido junto a cada release nueva.

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
