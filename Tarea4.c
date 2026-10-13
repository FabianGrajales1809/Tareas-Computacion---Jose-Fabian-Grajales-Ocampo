#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_TABLA 10

// Nodo para la lista enlazada
typedef struct Nodo {
    char *clave;
    int valor;
    struct Nodo *siguiente;
} Nodo;

// Estructura de la tabla hash
typedef struct {
    Nodo *cubetas[TAMANO_TABLA];
} TablaHash;


unsigned int funcion_hash(const char *clave) {
    unsigned int suma = 0;
    while (*clave) {
        suma += (unsigned char)(*clave);
        clave++;
    }
    return suma % TAMANO_TABLA;
}

// Inicializar la tabla hash
void inicializar(TablaHash *tabla) {
    for (int i = 0; i < TAMANO_TABLA; i++) {
        tabla->cubetas[i] = NULL;
    }
}

// Función insertar para las colisiones
void insertar(TablaHash *tabla, const char *clave, int valor) {
    unsigned int indice = funcion_hash(clave);
    Nodo *actual = tabla->cubetas[indice];

    // Buscar si ya existe EXACTAMENTE la misma clave y el mismo valor
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0 && actual->valor == valor) {
            printf("Aviso: El registro '%s' con valor %d ya existe. Ignorando.\n", clave, valor);
            return;
        }
        actual = actual->siguiente;
    }

    // agregarlo a la misma altura para la colisión
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->clave = strdup(clave);
    nuevo->valor = valor;
    // Se inserta al principio de la lista enlazada
    nuevo->siguiente = tabla->cubetas[indice];
    tabla->cubetas[indice] = nuevo;
    printf("Insertado: '%s' con valor  %d\n", clave, valor);
}

// Función buscar, se le pasa la clave y el valor
int* buscar(TablaHash *tabla, const char *clave, int valor) {
    unsigned int indice = funcion_hash(clave);
    Nodo *actual = tabla->cubetas[indice];

    while (actual != NULL) {
        // Al momento de buscarlo se diferencien los Ernestos
        if (strcmp(actual->clave, clave) == 0 && actual->valor == valor) {
            return &(actual->valor);
        }
        actual = actual->siguiente;
    }
    return NULL; // No encontrado
}

// Liberar memoria de la tabla
void liberar(TablaHash *tabla) {
    for (int i = 0; i < TAMANO_TABLA; i++) {
        Nodo *actual = tabla->cubetas[i];
        while (actual != NULL) {
            Nodo *temp = actual;
            actual = actual->siguiente;
            free(temp->clave);
            free(temp);
        }
        tabla->cubetas[i] = NULL;
    }
}

int main(void) {
    TablaHash mi_tabla;
    inicializar(&mi_tabla);

    printf("--- INICIANDO INSERCIONES ---\n");
    insertar(&mi_tabla, "eric", 18);
    insertar(&mi_tabla, "fabian", 27);
    insertar(&mi_tabla, "alhondra", 14);
    insertar(&mi_tabla, "malena", 29);
    insertar(&mi_tabla, "gabriel", 7);
    insertar(&mi_tabla, "yester", 11);
    insertar(&mi_tabla, "freddy", 38);
    insertar(&mi_tabla, "felix", 4);
    // Prueba de los "Ernestos"
    insertar(&mi_tabla, "ernesto", 10); // Ernesto #1
    insertar(&mi_tabla, "ernesto", 20); // Ernesto #2
    insertar(&mi_tabla, "ernesto", 10); // ignorar

    printf("\n--- INICIANDO BUSQUEDAS ---\n");
    // Buscamos al Ernesto de 10
    int *res1 = buscar(&mi_tabla, "ernesto", 10);
    if (res1 != NULL) {
        printf("Encontrado: 'ernesto' con valor %d\n", *res1);
    }

    // Buscamos al Ernesto de 20
    int *res2 = buscar(&mi_tabla, "ernesto", 20);
    if (res2 != NULL) {
        printf("Encontrado: 'ernesto' con valor %d\n", *res2);
    }

    // Buscamos a un Ernesto que no existe
    int *res3 = buscar(&mi_tabla, "ernesto", 30);
    if (res3 == NULL) {
        printf("No existe 'ernesto' con valor 30\n");
    }
    
    int *res4 = buscar(&mi_tabla, "fabian", 27);
    if (res4 != NULL) {
        printf("Encontrado: 'fabian' con valor %d\n", *res4);
    }
    
    int *res5 = buscar(&mi_tabla, "alhondra", 14);
    if (res5 != NULL) {
        printf("Encontrado: 'alhondra' con valor %d\n", *res5);
    }
    
    // Liberar la memoria
    liberar(&mi_tabla);
    printf("\nMemoria liberada\n");

    return 0;
}
