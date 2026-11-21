# Desafío 03: Sistema de gestión de tareas (becas)

Programa en C++ (consola) que gestiona solicitudes de beca como tareas. Incorpora estructuras de datos, punteros, ordenamiento por burbuja y búsquedas secuencial y binaria. Continuación del Desafío 02.

## Estructura del repositorio

```
src/Desafio3.cpp                         Código fuente
docs/Desafio3_Diagramas_de_flujo.pdf     Diagramas de flujo (7)
docs/Desafio3_Pseudocodigo.pdf           Pseudocódigo
README.md                                Este archivo
```

## Cómo compilar y ejecutar

**Con Dev-C++:** abrir `src/Desafio3.cpp`, presionar F11 (Compilar y ejecutar).

**Con g++ (terminal):**

```
g++ src/Desafio3.cpp -o desafio3
./desafio3
```

## Funcionalidades

| Opción | Descripción |
|--------|-------------|
| 1 | Agregar tarea (ID automático, validaciones y requisitos de la beca) |
| 2 | Actualizar tarea por ID (estado y días de vencimiento) |
| 3 | Mostrar tareas |
| 4 | Eliminar tarea por ID |
| 5 | Ordenar tareas por días de vencimiento (burbuja) |
| 6 | Buscar tarea por nombre (búsqueda secuencial) |
| 7 | Buscar tarea por ID (búsqueda binaria) |
| 8 | Salir |

El programa arranca con 4 tareas de prueba:

| ID | Nombre | Días | Estado |
|----|--------|------|--------|
| 1 | Beca Carlos | 15 | Preseleccionada |
| 2 | Beca Ana | 5 | Aprobada |
| 3 | Beca Diego | 25 | Preseleccionada |
| 4 | Beca Beatriz | 10 | Rechazada |

## Cómo probar el ordenamiento y la búsqueda

**Ordenamiento (opción 5)**
1. Elegir `5`.
2. Resultado esperado, de menor a mayor días: Beca Ana (5), Beca Beatriz (10), Beca Carlos (15), Beca Diego (25).

**Búsqueda secuencial por nombre (opción 6)**
1. Elegir `6` e ingresar `Beca Ana` (no distingue mayúsculas, funciona también `beca ana`).
2. Resultado esperado: se muestra la tarea con ID 2.
3. Repetir con un nombre inexistente (por ejemplo `xyz`). Resultado esperado: mensaje de que no se encontró ninguna tarea con ese nombre.

**Búsqueda binaria por ID (opción 7)**
1. Elegir `7` e ingresar `3`.
2. Resultado esperado: se muestra Beca Diego.
3. Repetir con un ID inexistente (por ejemplo `99`). Resultado esperado: mensaje de que no se encontró la tarea.
4. Nota: la búsqueda binaria requiere la lista ordenada, por eso el programa ordena por ID antes de buscar. Por este motivo, después de usar la opción 7 la lista queda ordenada por ID.

**Validaciones para probar**
- Agregar una tarea con nombre repetido, vacío, días menores o iguales a 0, o promedio menor a 9.5: el programa rechaza el ingreso.
- Actualizar o eliminar con un ID inexistente: muestra un error.

## Decisiones de diseño

- **Vector (`vector<Tarea>`):** permite agregar y eliminar tareas sin fijar un tamaño máximo y mantiene los datos contiguos en memoria.
- **Punteros:** las funciones de actualizar, eliminar, ordenar y buscar reciben la lista por puntero, para modificar o recorrer el vector original sin copiarlo.
- **Burbuja:** simple de implementar y de explicar. Complejidad O(n²), aceptable para una lista pequeña.
- **Búsqueda secuencial (por nombre):** O(n). No requiere orden previo, por eso se usa con el nombre, que no es el criterio de ordenamiento.
- **Búsqueda binaria (por ID):** O(log n) sobre la lista ordenada. Incluye el ordenamiento previo O(n²), por lo que conviene cuando se hacen muchas búsquedas sobre una lista que cambia poco.

## Cambios respecto del Desafío 02

- Se agregó la función `eliminarTarea` por ID.
- `actualizarTarea` y las demás funciones pasaron a recibir la lista por puntero.
- Se agregó `ordenarPorBurbuja` (por días de vencimiento) y `ordenarPorIdBurbuja` (auxiliar para la búsqueda binaria).
- Se agregaron `busquedaSecuencial` (por nombre) y `busquedaBinaria` (por ID).
- Se amplió el menú a 8 opciones.

## Control de versiones

El historial de commits del proyecto se puede ver con:

```
git log --oneline
```
