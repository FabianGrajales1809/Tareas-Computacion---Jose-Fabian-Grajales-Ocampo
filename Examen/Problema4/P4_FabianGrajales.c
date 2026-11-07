#include <stdio.h>
#include <stdlib.h>

// Estructura del nodo n-ario
typedef struct Nodo {
    int valor;
    struct Nodo** hijos; // Arreglo dinámico de apuntadores a los hijos
} Nodo;

// Función para crear un nuevo nodo
Nodo* crear_nodo(int valor, int n) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->valor = valor;
    nuevo->hijos = (Nodo**)malloc(n * sizeof(Nodo*));
    
    // Inicializar todos los hijos como nulos
    for (int i = 0; i < n; i++) {
        nuevo->hijos[i] = NULL;
    }
    return nuevo;
}

// Criterio de inserción recursiva: Busca el primer hueco disponible de izquierda a derecha
int insertar_recursivo(Nodo* actual, int valor, int n) {
    if (actual == NULL) return 0;

    // Intentar insertar en los hijos directos del nodo actual
    for (int i = 0; i < n; i++) {
        if (actual->hijos[i] == NULL) {
            actual->hijos[i] = crear_nodo(valor, n);
            return 1; // Inserción exitosa
        }
    }

    // Si todos los hijos directos están ocupados, intentar recursivamente en los subárboles
    for (int i = 0; i < n; i++) {
        if (insertar_recursivo(actual->hijos[i], valor, n)) {
            return 1; // Inserción exitosa en una rama inferior
        }
    }

    return 0; // No se pudo insertar
}

// Función recursiva para imprimir el árbol (Pre-orden)
void imprimir_arbol(Nodo* actual, int n, int nivel) {
    if (actual == NULL) return;

    // Imprimir sangría según el nivel de profundidad
    for (int i = 0; i < nivel; i++) printf("   ");
    printf("-> %d\n", actual->valor);

    // Llamadas recursivas para los hijos
    for (int i = 0; i < n; i++) {
        if (actual->hijos[i] != NULL) {
            imprimir_arbol(actual->hijos[i], n, nivel + 1);
        }
    }
}

// Liberar memoria recursivamente
void liberar_arbol(Nodo* actual, int n) {
    if (actual == NULL) return;
    for (int i = 0; i < n; i++) {
        liberar_arbol(actual->hijos[i], n);
    }
    free(actual->hijos);
    free(actual);
}

int main() {
    int n = 3; // Parámetro: cada nodo tendrá exactamente 3 hijos (n > 2)
    printf("--- ARBOL N-ARIO (n = %d) en C ---\n", n);

    // Crear la raíz
    Nodo* raiz = crear_nodo(10, n);

    // Insertar nodos (se usarán funciones recursivas)
    insertar_recursivo(raiz, 20, n);
    insertar_recursivo(raiz, 30, n);
    insertar_recursivo(raiz, 40, n); // Hasta aquí, la raíz se llena
    
    // Estos se insertarán como hijos del nodo '20' debido a la recursividad de izquierda a derecha
    insertar_recursivo(raiz, 50, n); 
    insertar_recursivo(raiz, 60, n);
    insertar_recursivo(raiz, 70, n);
    
    // Este pasará al nodo '30'
    insertar_recursivo(raiz, 80, n); 

    printf("Contenido del arbol:\n");
    imprimir_arbol(raiz, n, 0);

    liberar_arbol(raiz, n);
    return 0;
}
