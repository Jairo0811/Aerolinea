# ✈️ AerolineaCPP

Versión modernizada del proyecto académico **Aerolinea**, desarrollada con C++, Qt 6 Widgets y Microsoft SQL Server.

## Requisitos

- Qt 6.5 o superior con Qt Widgets y Qt SQL
- CMake 3.19 o superior
- MinGW 64-bit o MSVC compatible con Qt
- Microsoft SQL Server / SQL Server Express
- Microsoft ODBC Driver 17 for SQL Server

## Configuración de la base de datos

1. Ejecuta `Aerolinea.sql` en SQL Server Management Studio.
2. Revisa `config/database.ini`.
3. Cambia `server` por el nombre de tu instancia SQL Server cuando sea necesario.

Ejemplo:

```ini
[database]
driver=ODBC Driver 17 for SQL Server
server=localhost\SQLEXPRESS
database=AerolineaDB
trustedConnection=true
encrypt=true
trustServerCertificate=false
```

La aplicación busca la configuración en `config/database.ini`. Si no existe, utiliza `config/database.example.ini` como plantilla.

No guardes usuarios ni contraseñas en `database.ini`. Para autenticación SQL,
establece `trustedConnection=false` y proporciona las credenciales en las
variables de entorno `AEROLINEA_DB_USER` y `AEROLINEA_DB_PASSWORD`. La
aplicación rechaza conexiones sin cifrado; conserva
`trustServerCertificate=false` fuera de un entorno local de desarrollo y usa
un certificado válido en SQL Server.

El script crea el rol `AerolineaReader`, que solo posee permisos `SELECT` sobre
las cuatro tablas usadas por la aplicación. Asigna a ese rol el usuario de
Windows o SQL utilizado por el programa; no uses una cuenta administradora.

## Seguridad y límites de confianza

AerolineaCPP es una aplicación de escritorio de consulta y no expone rutas HTTP
ni una API. La autenticación corresponde a la sesión de Windows y a SQL Server.
Las consultas son estáticas o parametrizadas; la configuración ODBC se valida y
se escapa antes de formar la cadena de conexión. Además, la aplicación limita a
1,000 los registros cargados por entidad y descarta texto o valores numéricos
fuera de los rangos definidos por el esquema.

## Compilación en Qt Creator

1. Abre `CMakeLists.txt` desde Qt Creator.
2. Selecciona el kit Desktop Qt 6.x MinGW 64-bit.
3. Ejecuta **Run CMake**.
4. Compila y ejecuta el proyecto.

CMake copia automáticamente la carpeta `config` junto al ejecutable.

## Funcionalidades

- Consulta dinámica de destinos, rutas, aeronaves y vuelos
- Búsqueda de rutas con escalas
- Distancia y duración total
- Precio total del viaje
- Información del vuelo y aeronave asignada
- Créditos históricos del proyecto original ITLA 2018-C1
