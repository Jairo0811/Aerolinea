# ✈️ Aerolinea Legacy

Versión legacy del sistema de aerolínea desarrollado originalmente en abril de 2018 para la asignatura **Estructuras de Datos (SOF-012)** del **Instituto Tecnológico de Las Américas (ITLA)**.

## 👨‍💻 Integrantes

- Francis Jairo Matías Rosario — 2015-2984
- Jorge de Jesús Torres Pérez — 2016-3515
- Sebastian Donastor Hernández — 2016-3607

## 🎓 Profesor

Raydelto Hernández

## 🧰 Tecnologías

- C++
- Code::Blocks
- Listas enlazadas
- Programación estructurada

## 📌 Funcionalidades

- Registrar destinos
- Calcular cantidad de vuelos entre dos destinos
- Buscar ruta entre origen y destino
- Listar destinos registrados
- Listar rutas registradas

## 🔧 Correcciones realizadas

- Eliminación de fugas de memoria por uso innecesario de `new`
- Destructor para liberar la lista enlazada
- Mejor manejo de entradas con `getline`
- Opción para salir del sistema
- Validaciones de origen y destino
- Menú de consola más claro
- Separación del código en archivos `.h` y `.cpp`
- Compatibilidad mejorada con compiladores modernos

## ▶️ Ejecución

Abrir `prueba.cbp` en Code::Blocks y compilar el proyecto.

También puede compilarse desde terminal:

```bash
g++ main.cpp paises.cpp -o AerolineaLegacy
```

## 📚 Nota

Esta carpeta conserva la versión histórica del proyecto. La versión modernizada con Qt 6 y SQL Server se encuentra en:

```text
../modern-qt-sqlserver/
```
