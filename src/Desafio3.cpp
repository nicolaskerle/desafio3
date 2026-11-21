#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <cctype>

using namespace std;

struct Tarea {
    int id;
    string nombre;
    string descripcion;
    int diasVencimiento;
    string estado;
};

// Prototipos de funciones
string aMinusculas(string texto);
bool existeTarea(const vector<Tarea>& listaTareas, const string& nombre);
bool agregarTarea(vector<Tarea>& listaTareas, string nombre, string descripcion,
                  int dias, float promedio, char respuesta);
bool actualizarTarea(vector<Tarea>* listaTareas, int idBuscar, string nuevoEstado, int nuevosDias);
bool eliminarTarea(vector<Tarea>* listaTareas, int id);
void mostrarTareas(const vector<Tarea>& listaTareas);
void ordenarPorBurbuja(vector<Tarea>* listaTareas);
void ordenarPorIdBurbuja(vector<Tarea>* listaTareas);
int busquedaSecuencial(const vector<Tarea>* listaTareas, const string& nombreBuscar);
int busquedaBinaria(vector<Tarea>* listaTareas, int idBuscar);

// Funcion auxiliar para convertir texto a minusculas (evita duplicados con distinta capitalizacion)
string aMinusculas(string texto) {
    for (size_t i = 0; i < texto.length(); i++) {
        texto[i] = tolower(texto[i]);
    }
    return texto;
}

// Funcion auxiliar para validar que no haya nombres duplicados
bool existeTarea(const vector<Tarea>& listaTareas, const string& nombre) {
    for (size_t i = 0; i < listaTareas.size(); i++) {
        if (aMinusculas(listaTareas[i].nombre) == aMinusculas(nombre)) {
            return true;
        }
    }
    return false;
}

// Funcion modular para agregar una tarea con ID correlativo automatico
bool agregarTarea(vector<Tarea>& listaTareas, string nombre, string descripcion,
                  int dias, float promedio, char respuesta) {
    if (nombre.empty()) {
        printf("Error: el nombre de la tarea no puede estar vacio.\n");
        return false;
    }
    if (existeTarea(listaTareas, nombre)) {
        printf("Error: ya existe una tarea con el nombre '%s'.\n", nombre.c_str());
        return false;
    }
    if (dias <= 0) {
        printf("Error: la cantidad de dias para el vencimiento debe ser mayor a 0.\n");
        return false;
    }

    // Requisitos de la beca
    if (promedio < 9.5) {
        printf("No cumple los requisitos: el promedio debe ser 9.5 o mas.\n");
        return false;
    }
    if (respuesta == 'N' || respuesta == 'n') {
        printf("No cumple los requisitos: debe ser ciudadano argentino y residir en zona sur.\n");
        return false;
    }

    // Calculo automatico del siguiente ID disponible
    int maxId = 0;
    for (size_t i = 0; i < listaTareas.size(); i++) {
        if (listaTareas[i].id > maxId) {
            maxId = listaTareas[i].id;
        }
    }

    Tarea nueva;
    nueva.id = maxId + 1;
    nueva.nombre = nombre;
    nueva.descripcion = descripcion;
    nueva.diasVencimiento = dias;
    nueva.estado = "Preseleccionada";

    listaTareas.push_back(nueva);
    return true;
}

// Funcion modular para actualizar una tarea por ID usando punteros
bool actualizarTarea(vector<Tarea>* listaTareas, int idBuscar, string nuevoEstado, int nuevosDias) {
    if (listaTareas == NULL || listaTareas->empty()) {
        printf("Error: no hay tareas registradas.\n");
        return false;
    }

    if (nuevosDias <= 0) {
        printf("Error: los dias de vencimiento deben ser mayores a 0.\n");
        return false;
    }

    for (size_t i = 0; i < listaTareas->size(); i++) {
        if ((*listaTareas)[i].id == idBuscar) {
            (*listaTareas)[i].estado = nuevoEstado;
            (*listaTareas)[i].diasVencimiento = nuevosDias;
            return true;
        }
    }

    printf("Error: no se encontro ninguna tarea con el ID %d.\n", idBuscar);
    return false;
}

// Funcion modular para mostrar todas las tareas
void mostrarTareas(const vector<Tarea>& listaTareas) {
    if (listaTareas.empty()) {
        printf("\nNo hay tareas registradas.\n");
        return;
    }

    printf("\n=== LISTA DE TAREAS ===\n");
    for (size_t i = 0; i < listaTareas.size(); i++) {
        printf("ID: %d\n", listaTareas[i].id);
        printf("Nombre: %s\n", listaTareas[i].nombre.c_str());
        printf("Descripcion: %s\n", listaTareas[i].descripcion.c_str());
        printf("Dias para el vencimiento: %d\n", listaTareas[i].diasVencimiento);
        printf("Estado: %s\n", listaTareas[i].estado.c_str());
        printf("-----------------------------\n");
    }
}

// Funcion modular para eliminar una tarea por ID usando punteros
bool eliminarTarea(vector<Tarea>* listaTareas, int id) {
    if (listaTareas == NULL || listaTareas->empty()) {
        printf("No hay tareas para eliminar.\n");
        return false;
    }

    for (size_t i = 0; i < listaTareas->size(); i++) {
        if ((*listaTareas)[i].id == id) {
            listaTareas->erase(listaTareas->begin() + i);
            return true;
        }
    }

    printf("Error: no se encontro ninguna tarea con el ID %d.\n", id);
    return false;
}

// Ordenamiento por burbuja segun dias de vencimiento - Complejidad: O(n^2)
void ordenarPorBurbuja(vector<Tarea>* listaTareas) {
    if (listaTareas == NULL || listaTareas->size() <= 1) {
        return;
    }

    size_t n = listaTareas->size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - 1 - i; j++) {
            if ((*listaTareas)[j].diasVencimiento > (*listaTareas)[j + 1].diasVencimiento) {
                Tarea temp = (*listaTareas)[j];
                (*listaTareas)[j] = (*listaTareas)[j + 1];
                (*listaTareas)[j + 1] = temp;
            }
        }
    }
}

// Ordenamiento auxiliar ascendente por ID (requisito previo para busqueda binaria)
void ordenarPorIdBurbuja(vector<Tarea>* listaTareas) {
    if (listaTareas == NULL || listaTareas->size() <= 1) {
        return;
    }

    size_t n = listaTareas->size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - 1 - i; j++) {
            if ((*listaTareas)[j].id > (*listaTareas)[j + 1].id) {
                Tarea temp = (*listaTareas)[j];
                (*listaTareas)[j] = (*listaTareas)[j + 1];
                (*listaTareas)[j + 1] = temp;
            }
        }
    }
}

// Busqueda secuencial por nombre (sin distinguir mayusculas) - Complejidad: O(n)
int busquedaSecuencial(const vector<Tarea>* listaTareas, const string& nombreBuscar) {
    if (listaTareas == NULL || listaTareas->empty()) {
        return -1;
    }

    string buscado = aMinusculas(nombreBuscar);
    for (size_t i = 0; i < listaTareas->size(); i++) {
        if (aMinusculas((*listaTareas)[i].nombre) == buscado) {
            return (int)i;
        }
    }
    return -1;
}

// Busqueda binaria por ID - Complejidad: O(log n) tras ordenar O(n^2)
int busquedaBinaria(vector<Tarea>* listaTareas, int idBuscar) {
    if (listaTareas == NULL || listaTareas->empty()) {
        return -1;
    }

    // Requisito: ordenar previamente por ID antes de aplicar busqueda binaria
    ordenarPorIdBurbuja(listaTareas);

    int inicio = 0;
    int fin = (int)listaTareas->size() - 1;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;

        if ((*listaTareas)[medio].id == idBuscar) {
            return medio;
        } else if ((*listaTareas)[medio].id < idBuscar) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }

    return -1;
}

int main() {
    vector<Tarea> listaTareas;

    // Carga inicial de datos de prueba
    Tarea t1;
    t1.id = 1;
    t1.nombre = "Beca Carlos";
    t1.descripcion = "Solicitud ingenieria";
    t1.diasVencimiento = 15;
    t1.estado = "Preseleccionada";
    listaTareas.push_back(t1);

    Tarea t2;
    t2.id = 2;
    t2.nombre = "Beca Ana";
    t2.descripcion = "Solicitud medicina";
    t2.diasVencimiento = 5;
    t2.estado = "Aprobada";
    listaTareas.push_back(t2);

    Tarea t3;
    t3.id = 3;
    t3.nombre = "Beca Diego";
    t3.descripcion = "Solicitud derecho";
    t3.diasVencimiento = 25;
    t3.estado = "Preseleccionada";
    listaTareas.push_back(t3);

    Tarea t4;
    t4.id = 4;
    t4.nombre = "Beca Beatriz";
    t4.descripcion = "Solicitud arquitectura";
    t4.diasVencimiento = 10;
    t4.estado = "Rechazada";
    listaTareas.push_back(t4);

    int opcion = 0;

    do {
        printf("\n=========================================\n");
        printf("      SISTEMA DE GESTION DE TAREAS\n");
        printf("=========================================\n\n");
        printf("  1. Agregar tarea\n");
        printf("  2. Actualizar tarea por ID\n");
        printf("  3. Mostrar tareas\n");
        printf("  4. Eliminar tarea por ID\n");
        printf("  5. Ordenar tareas por vencimiento (Burbuja)\n");
        printf("  6. Buscar tarea por nombre (Secuencial)\n");
        printf("  7. Buscar tarea por ID (Binaria)\n");
        printf("  8. Salir\n\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            printf("Error: ingrese un numero valido.\n");
            continue;
        }
        while (getchar() != '\n'); // Limpiar buffer

        if (opcion == 1) {
            string nombre;
            printf("\nIngrese el nombre: ");
            getline(cin, nombre);
            while (nombre.empty() || existeTarea(listaTareas, nombre)) {
                if (nombre.empty()) {
                    printf("Error: el nombre no puede estar vacio.\n");
                } else {
                    printf("Error: ya existe una tarea con ese nombre.\n");
                }
                printf("Ingrese el nombre: ");
                getline(cin, nombre);
            }

            string descripcion;
            printf("Ingresa la descripcion de la tarea (opcional): ");
            getline(cin, descripcion);

            int dias, leidos;
            printf("Ingresa en cuantos dias vence la tarea: ");
            leidos = scanf("%d", &dias);
            while (getchar() != '\n');

            while (leidos != 1 || dias <= 0) {
                if (leidos != 1) printf("Error: ingrese un numero entero.\n");
                else printf("Error: los dias deben ser mayores a 0.\n");

                printf("Ingresa en cuantos dias vence la tarea: ");
                leidos = scanf("%d", &dias);
                while (getchar() != '\n');
            }

            float promedio;
            int leidoPromedio;
            printf("Ingresa el promedio (0 a 10): ");
            leidoPromedio = scanf("%f", &promedio);
            while (getchar() != '\n');

            while (leidoPromedio != 1 || promedio < 0 || promedio > 10) {
                if (leidoPromedio != 1) printf("Error: ingrese un numero (no letras).\n");
                else printf("Error: el promedio debe estar entre 0 y 10.\n");

                printf("Ingresa el promedio (0 a 10): ");
                leidoPromedio = scanf("%f", &promedio);
                while (getchar() != '\n');
            }

            if (promedio < 9.5) {
                printf("No cumple los requisitos: el promedio debe ser 9.5 o mas.\n");
                continue;
            }

            char respuesta;
            printf("Es ciudadano argentino y reside en zona sur? (S/N): ");
            scanf(" %c", &respuesta);
            while (getchar() != '\n');

            while (respuesta != 'S' && respuesta != 's' && respuesta != 'N' && respuesta != 'n') {
                printf("Error: responda S o N.\n");
                printf("Es ciudadano argentino y reside en zona sur? (S/N): ");
                scanf(" %c", &respuesta);
                while (getchar() != '\n');
            }

            if (agregarTarea(listaTareas, nombre, descripcion, dias, promedio, respuesta)) {
                printf("Solicitud registrada con exito.\n");
            }

        } else if (opcion == 2) {
            if (listaTareas.empty()) {
                printf("No hay tareas para actualizar.\n");
                continue;
            }

            mostrarTareas(listaTareas);

            int idBuscar, nuevosDias, leidos;
            string nuevoEstado;

            printf("\nIngrese el ID de la tarea a actualizar: ");
            leidos = scanf("%d", &idBuscar);
            while (getchar() != '\n');

            while (leidos != 1 || idBuscar <= 0) {
                printf("Error: ingrese un numero de ID valido: ");
                leidos = scanf("%d", &idBuscar);
                while (getchar() != '\n');
            }

            printf("Ingrese el nuevo estado (Preseleccionada, Aprobada o Rechazada): ");
            getline(cin, nuevoEstado);

            printf("Ingrese los nuevos dias para el vencimiento: ");
            leidos = scanf("%d", &nuevosDias);
            while (getchar() != '\n');

            while (leidos != 1 || nuevosDias <= 0) {
                printf("Error: ingrese un numero mayor a 0: ");
                leidos = scanf("%d", &nuevosDias);
                while (getchar() != '\n');
            }

            if (actualizarTarea(&listaTareas, idBuscar, nuevoEstado, nuevosDias)) {
                printf("Tarea actualizada correctamente.\n");
            }

        } else if (opcion == 3) {
            mostrarTareas(listaTareas);

        } else if (opcion == 4) {
            if (listaTareas.empty()) {
                printf("No hay tareas para eliminar.\n");
                continue;
            }

            mostrarTareas(listaTareas);

            int id, leidos;
            printf("\nIngrese el ID de la tarea a eliminar: ");
            leidos = scanf("%d", &id);
            while (getchar() != '\n');

            while (leidos != 1 || id <= 0) {
                printf("Error: ingrese un numero de ID valido: ");
                leidos = scanf("%d", &id);
                while (getchar() != '\n');
            }

            if (eliminarTarea(&listaTareas, id)) {
                printf("Tarea eliminada correctamente.\n");
            }

        } else if (opcion == 5) {
            if (listaTareas.empty()) {
                printf("No hay tareas para ordenar.\n");
                continue;
            }
            ordenarPorBurbuja(&listaTareas);
            printf("\nTareas ordenadas por dias de vencimiento:\n");
            mostrarTareas(listaTareas);

        } else if (opcion == 6) {
            if (listaTareas.empty()) {
                printf("No hay tareas registradas.\n");
                continue;
            }

            string nombreBuscar;
            printf("\nIngrese el nombre de la tarea a buscar (Secuencial): ");
            getline(cin, nombreBuscar);

            while (nombreBuscar.empty()) {
                printf("Error: el nombre no puede estar vacio.\n");
                printf("Ingrese el nombre de la tarea a buscar (Secuencial): ");
                getline(cin, nombreBuscar);
            }

            int pos = busquedaSecuencial(&listaTareas, nombreBuscar);
            if (pos != -1) {
                printf("\n=== TAREA ENCONTRADA ===\n");
                printf("ID: %d\n", listaTareas[pos].id);
                printf("Nombre: %s\n", listaTareas[pos].nombre.c_str());
                printf("Descripcion: %s\n", listaTareas[pos].descripcion.c_str());
                printf("Dias para el vencimiento: %d\n", listaTareas[pos].diasVencimiento);
                printf("Estado: %s\n", listaTareas[pos].estado.c_str());
                printf("-------------------------\n");
            } else {
                printf("No se encontro ninguna tarea con el nombre '%s'.\n", nombreBuscar.c_str());
            }

        } else if (opcion == 7) {
            if (listaTareas.empty()) {
                printf("No hay tareas registradas.\n");
                continue;
            }

            int idBuscar, leidos;
            printf("\nIngrese el ID de la tarea a buscar (Binaria): ");
            leidos = scanf("%d", &idBuscar);
            while (getchar() != '\n');

            while (leidos != 1 || idBuscar <= 0) {
                printf("Error: ingrese un numero de ID valido: ");
                leidos = scanf("%d", &idBuscar);
                while (getchar() != '\n');
            }

            int pos = busquedaBinaria(&listaTareas, idBuscar);
            if (pos != -1) {
                printf("\n=== TAREA ENCONTRADA (Lista ordenada por ID) ===\n");
                printf("ID: %d\n", listaTareas[pos].id);
                printf("Nombre: %s\n", listaTareas[pos].nombre.c_str());
                printf("Descripcion: %s\n", listaTareas[pos].descripcion.c_str());
                printf("Dias para el vencimiento: %d\n", listaTareas[pos].diasVencimiento);
                printf("Estado: %s\n", listaTareas[pos].estado.c_str());
                printf("------------------------------------------------\n");
            } else {
                printf("No se encontro ninguna tarea con el ID %d.\n", idBuscar);
            }

        } else if (opcion != 8) {
            printf("Opcion invalida.\n");
        }

    } while (opcion != 8);

    printf("Saliendo del programa...\n");
    return 0;
}
