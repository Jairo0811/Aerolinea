IF DB_ID(N'AerolineaDB') IS NULL
BEGIN
    EXEC(N'CREATE DATABASE AerolineaDB;');
END;
GO

USE AerolineaDB;
GO

SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRY
    BEGIN TRANSACTION;

    IF OBJECT_ID(N'dbo.Destinos', N'U') IS NULL
    BEGIN
        CREATE TABLE dbo.Destinos
        (
            Id INT IDENTITY(1,1) NOT NULL
                CONSTRAINT PK_Destinos PRIMARY KEY,
            Nombre NVARCHAR(100) NOT NULL
        );
    END;

    IF OBJECT_ID(N'dbo.Aeronaves', N'U') IS NULL
    BEGIN
        CREATE TABLE dbo.Aeronaves
        (
            Id INT IDENTITY(1,1) NOT NULL
                CONSTRAINT PK_Aeronaves PRIMARY KEY,
            Modelo NVARCHAR(100) NOT NULL,
            Capacidad INT NOT NULL,
            Fabricante NVARCHAR(100) NOT NULL
        );
    END;

    IF OBJECT_ID(N'dbo.Rutas', N'U') IS NULL
    BEGIN
        CREATE TABLE dbo.Rutas
        (
            Id INT IDENTITY(1,1) NOT NULL
                CONSTRAINT PK_Rutas PRIMARY KEY,
            OrigenId INT NOT NULL,
            DestinoId INT NOT NULL,
            DistanciaKm INT NOT NULL,
            DuracionMinutos INT NOT NULL
        );
    END;

    IF OBJECT_ID(N'dbo.Vuelos', N'U') IS NULL
    BEGIN
        CREATE TABLE dbo.Vuelos
        (
            Id INT IDENTITY(1,1) NOT NULL
                CONSTRAINT PK_Vuelos PRIMARY KEY,
            RutaId INT NOT NULL,
            AeronaveId INT NOT NULL,
            CodigoVuelo NVARCHAR(20) NOT NULL,
            Aerolinea NVARCHAR(100) NOT NULL,
            Precio DECIMAL(10,2) NOT NULL
        );
    END;

    IF NOT EXISTS (SELECT 1 FROM sys.check_constraints WHERE name = N'CK_Destinos_Nombre')
        ALTER TABLE dbo.Destinos WITH CHECK ADD CONSTRAINT CK_Destinos_Nombre
            CHECK (LEN(LTRIM(RTRIM(Nombre))) BETWEEN 1 AND 100);

    IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'UX_Destinos_Nombre' AND object_id = OBJECT_ID(N'dbo.Destinos'))
        CREATE UNIQUE INDEX UX_Destinos_Nombre ON dbo.Destinos(Nombre);

    IF NOT EXISTS (SELECT 1 FROM sys.check_constraints WHERE name = N'CK_Aeronaves_Valores')
        ALTER TABLE dbo.Aeronaves WITH CHECK ADD CONSTRAINT CK_Aeronaves_Valores
            CHECK (
                LEN(LTRIM(RTRIM(Modelo))) BETWEEN 1 AND 100 AND
                LEN(LTRIM(RTRIM(Fabricante))) BETWEEN 1 AND 100 AND
                Capacidad BETWEEN 1 AND 1000
            );

    IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'UX_Aeronaves_Modelo' AND object_id = OBJECT_ID(N'dbo.Aeronaves'))
        CREATE UNIQUE INDEX UX_Aeronaves_Modelo ON dbo.Aeronaves(Modelo);

    IF NOT EXISTS (SELECT 1 FROM sys.check_constraints WHERE name = N'CK_Rutas_Valores')
        ALTER TABLE dbo.Rutas WITH CHECK ADD CONSTRAINT CK_Rutas_Valores
            CHECK (
                OrigenId <> DestinoId AND
                DistanciaKm BETWEEN 1 AND 25000 AND
                DuracionMinutos BETWEEN 1 AND 2880
            );

    IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'UX_Rutas_Origen_Destino' AND object_id = OBJECT_ID(N'dbo.Rutas'))
        CREATE UNIQUE INDEX UX_Rutas_Origen_Destino
            ON dbo.Rutas(OrigenId, DestinoId);

    IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = N'FK_Rutas_Origen')
        ALTER TABLE dbo.Rutas WITH CHECK ADD CONSTRAINT FK_Rutas_Origen
            FOREIGN KEY (OrigenId) REFERENCES dbo.Destinos(Id);

    IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = N'FK_Rutas_Destino')
        ALTER TABLE dbo.Rutas WITH CHECK ADD CONSTRAINT FK_Rutas_Destino
            FOREIGN KEY (DestinoId) REFERENCES dbo.Destinos(Id);

    IF NOT EXISTS (SELECT 1 FROM sys.check_constraints WHERE name = N'CK_Vuelos_Valores')
        ALTER TABLE dbo.Vuelos WITH CHECK ADD CONSTRAINT CK_Vuelos_Valores
            CHECK (
                LEN(LTRIM(RTRIM(CodigoVuelo))) BETWEEN 1 AND 20 AND
                LEN(LTRIM(RTRIM(Aerolinea))) BETWEEN 1 AND 100 AND
                Precio BETWEEN 0 AND 1000000
            );

    IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = N'UX_Vuelos_Codigo' AND object_id = OBJECT_ID(N'dbo.Vuelos'))
        CREATE UNIQUE INDEX UX_Vuelos_Codigo ON dbo.Vuelos(CodigoVuelo);

    IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = N'FK_Vuelos_Ruta')
        ALTER TABLE dbo.Vuelos WITH CHECK ADD CONSTRAINT FK_Vuelos_Ruta
            FOREIGN KEY (RutaId) REFERENCES dbo.Rutas(Id);

    IF NOT EXISTS (SELECT 1 FROM sys.foreign_keys WHERE name = N'FK_Vuelos_Aeronave')
        ALTER TABLE dbo.Vuelos WITH CHECK ADD CONSTRAINT FK_Vuelos_Aeronave
            FOREIGN KEY (AeronaveId) REFERENCES dbo.Aeronaves(Id);

    INSERT INTO dbo.Destinos (Nombre)
    SELECT seed.Nombre
    FROM (VALUES
        (N'Miami'),
        (N'Orlando'),
        (N'Santo Domingo'),
        (N'San Francisco'),
        (N'Nueva York')
    ) AS seed(Nombre)
    WHERE NOT EXISTS (
        SELECT 1 FROM dbo.Destinos existing WHERE existing.Nombre = seed.Nombre
    );

    INSERT INTO dbo.Aeronaves (Modelo, Capacidad, Fabricante)
    SELECT seed.Modelo, seed.Capacidad, seed.Fabricante
    FROM (VALUES
        (N'Boeing 737', 189, N'Boeing'),
        (N'Airbus A320', 180, N'Airbus'),
        (N'Embraer E195', 132, N'Embraer')
    ) AS seed(Modelo, Capacidad, Fabricante)
    WHERE NOT EXISTS (
        SELECT 1 FROM dbo.Aeronaves existing WHERE existing.Modelo = seed.Modelo
    );

    INSERT INTO dbo.Rutas (OrigenId, DestinoId, DistanciaKm, DuracionMinutos)
    SELECT origen.Id, destino.Id, seed.DistanciaKm, seed.DuracionMinutos
    FROM (VALUES
        (N'Miami', N'Orlando', 378, 70),
        (N'Orlando', N'Santo Domingo', 1600, 150),
        (N'Santo Domingo', N'San Francisco', 5400, 390),
        (N'San Francisco', N'Miami', 4150, 330),
        (N'Nueva York', N'Santo Domingo', 2500, 240),
        (N'Nueva York', N'Miami', 2100, 180),
        (N'San Francisco', N'Nueva York', 4200, 360)
    ) AS seed(Origen, Destino, DistanciaKm, DuracionMinutos)
    INNER JOIN dbo.Destinos origen ON origen.Nombre = seed.Origen
    INNER JOIN dbo.Destinos destino ON destino.Nombre = seed.Destino
    WHERE NOT EXISTS (
        SELECT 1 FROM dbo.Rutas existing
        WHERE existing.OrigenId = origen.Id AND existing.DestinoId = destino.Id
    );

    INSERT INTO dbo.Vuelos
        (RutaId, AeronaveId, CodigoVuelo, Aerolinea, Precio)
    SELECT ruta.Id, aeronave.Id, seed.CodigoVuelo, seed.Aerolinea, seed.Precio
    FROM (VALUES
        (N'Miami', N'Orlando', N'Boeing 737', N'AA101', N'American Airlines', CAST(250 AS DECIMAL(10,2))),
        (N'Orlando', N'Santo Domingo', N'Airbus A320', N'JB205', N'JetBlue', CAST(350 AS DECIMAL(10,2))),
        (N'Santo Domingo', N'San Francisco', N'Boeing 737', N'UA450', N'United Airlines', CAST(700 AS DECIMAL(10,2))),
        (N'San Francisco', N'Miami', N'Embraer E195', N'DL330', N'Delta Airlines', CAST(620 AS DECIMAL(10,2))),
        (N'Nueva York', N'Santo Domingo', N'Airbus A320', N'AA900', N'American Airlines', CAST(420 AS DECIMAL(10,2))),
        (N'San Francisco', N'Nueva York', N'Boeing 737', N'AA950', N'American Airlines', CAST(650 AS DECIMAL(10,2)))
    ) AS seed(Origen, Destino, Modelo, CodigoVuelo, Aerolinea, Precio)
    INNER JOIN dbo.Destinos origen ON origen.Nombre = seed.Origen
    INNER JOIN dbo.Destinos destino ON destino.Nombre = seed.Destino
    INNER JOIN dbo.Rutas ruta
        ON ruta.OrigenId = origen.Id AND ruta.DestinoId = destino.Id
    INNER JOIN dbo.Aeronaves aeronave ON aeronave.Modelo = seed.Modelo
    WHERE NOT EXISTS (
        SELECT 1 FROM dbo.Vuelos existing
        WHERE existing.CodigoVuelo = seed.CodigoVuelo
    );

    IF DATABASE_PRINCIPAL_ID(N'AerolineaReader') IS NULL
        CREATE ROLE AerolineaReader AUTHORIZATION dbo;

    GRANT SELECT ON dbo.Destinos TO AerolineaReader;
    GRANT SELECT ON dbo.Rutas TO AerolineaReader;
    GRANT SELECT ON dbo.Aeronaves TO AerolineaReader;
    GRANT SELECT ON dbo.Vuelos TO AerolineaReader;

    COMMIT TRANSACTION;
END TRY
BEGIN CATCH
    IF XACT_STATE() <> 0
        ROLLBACK TRANSACTION;
    THROW;
END CATCH;
GO

-- Asigna únicamente el rol de lectura al principal usado por la aplicación.
-- Ejemplo para autenticación integrada (ejecutar con el usuario real):
-- CREATE USER [DOMINIO\usuario] FROM WINDOWS;
-- ALTER ROLE AerolineaReader ADD MEMBER [DOMINIO\usuario];
