CREATE DATABASE AerolineaDB;
GO

CREATE TABLE Destinos
(
    Id INT IDENTITY(1,1) PRIMARY KEY,
    Nombre NVARCHAR(100) NOT NULL
);

INSERT INTO Destinos (Nombre)
VALUES
('Miami'),
('Orlando'),
('Santo Domingo'),
('San Francisco');

INSERT INTO Rutas (OrigenId, DestinoId, DistanciaKm, DuracionMinutos)
VALUES
(1, 2, 378, 70),
(2, 3, 1600, 150),
(3, 4, 5400, 390),
(4, 1, 4150, 330);

CREATE TABLE Rutas
(
    Id INT IDENTITY(1,1) PRIMARY KEY,
    OrigenId INT NOT NULL,
    DestinoId INT NOT NULL,
    DistanciaKm INT NOT NULL,
    DuracionMinutos INT NOT NULL
);


SELECT * FROM Destinos;
SELECT * FROM Rutas;


INSERT INTO Destinos (Nombre)
VALUES ('Nueva York');

INSERT INTO Rutas (OrigenId, DestinoId, DistanciaKm, DuracionMinutos)
VALUES
(5, 3, 2500, 240);

INSERT INTO Rutas
(
    OrigenId,
    DestinoId,
    DistanciaKm,
    DuracionMinutos
)
VALUES
(
    5, -- Nueva York
    1, -- Miami
    2100,
    180
);

CREATE TABLE Aeronaves
(
    Id INT IDENTITY(1,1) PRIMARY KEY,
    Modelo NVARCHAR(100) NOT NULL,
    Capacidad INT NOT NULL,
    Fabricante NVARCHAR(100) NOT NULL
);

INSERT INTO Aeronaves (Modelo, Capacidad, Fabricante)
VALUES
('Boeing 737', 189, 'Boeing'),
('Airbus A320', 180, 'Airbus'),
('Embraer E195', 132, 'Embraer');

SELECT * FROM Aeronaves;


CREATE TABLE Vuelos
(
    Id INT IDENTITY(1,1) PRIMARY KEY,

    RutaId INT NOT NULL,
    AeronaveId INT NOT NULL,

    CodigoVuelo NVARCHAR(20) NOT NULL,
    Aerolinea NVARCHAR(100) NOT NULL,
    Precio DECIMAL(10,2) NOT NULL,

    FOREIGN KEY (RutaId)
        REFERENCES Rutas(Id),

    FOREIGN KEY (AeronaveId)
        REFERENCES Aeronaves(Id)
);


INSERT INTO Vuelos
(
    RutaId,
    AeronaveId,
    CodigoVuelo,
    Aerolinea,
    Precio
)
VALUES
(1,1,'AA101','American Airlines',250),
(2,2,'JB205','JetBlue',350),
(3,1,'UA450','United Airlines',700),
(4,3,'DL330','Delta Airlines',620),
(5,2,'AA900','American Airlines',420);

SELECT * FROM Vuelos;

SELECT *
FROM Destinos
ORDER BY Id;

SELECT *
FROM Rutas
ORDER BY Id;

INSERT INTO Rutas
(
    OrigenId,
    DestinoId,
    DistanciaKm,
    DuracionMinutos
)
VALUES
(
    4, -- San Francisco
    5, -- Nueva York
    4200,
    360
);
DELETE FROM Rutas
WHERE Id IN (8, 9);
SELECT * FROM Rutas;
INSERT INTO Rutas
(
    OrigenId,
    DestinoId,
    DistanciaKm,
    DuracionMinutos
)
VALUES
(
    4, -- San Francisco
    5, -- Nueva York
    4200,
    360
);

INSERT INTO Vuelos
(
    RutaId,
    AeronaveId,
    CodigoVuelo,
    Aerolinea,
    Precio
)
VALUES
(
    7,
    1,
    'AA950',
    'American Airlines',
    650
);