#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct comparte {
    char nombre[50];
    char direccion[50];
    int  edad;
    int n_elemento; 
    struct comparte *next;
    struct comparte *prev;
} Registro; 

typedef struct l {
    Registro *Inicial;
    Registro *Final;
    int len;
} Lista;

// Crea un nodo vacío en memoria
Registro *crear() {
    Registro *T = (Registro *)malloc(sizeof(Registro));
    T->next = NULL;
    T->prev = NULL;
    return T;
}

// Inicializa la lista con sus nodos centinela
void crear_lista(Lista *lista) {
    Registro *N_inicial = crear();
    Registro *N_final = crear();
    
    lista->Inicial = N_inicial;
    lista->Final = N_final;
    lista->len = 0;
    
    lista->Inicial->next = N_final;
    lista->Inicial->prev = NULL;
    
    lista->Final->prev = N_inicial;
    lista->Final->next = NULL;
}

// Cambiamos char[50] por const char *
int agregar(Lista *lista, int n_elemento, const char *nombre, const char *direccion, int edad) {
      
    Registro *N;
    N = crear();
    
    if(N == NULL) return 0;
    
    strcpy(N->nombre, nombre);
    strcpy(N->direccion, direccion);
    N->edad = edad;
    N->n_elemento = n_elemento;
    
    // Conectar el nuevo nodo
    N->prev = lista->Final->prev;
    N->next = lista->Final;

    lista->Final->prev->next = N;    
    lista->Final->prev = N;
    
    lista->len++;
    return 1;
}

// EXTRAER FIFO (Cola): El primero que entró es el primero que sale
Registro* extraer_fifo(Lista *lista) {
    if (lista->Inicial->next == lista->Final) {
        return NULL; // La lista está vacía
    }
    
    Registro *primero = lista->Inicial->next;
    
    // Desenlazamos el primer nodo
    lista->Inicial->next = primero->next;
    primero->next->prev = lista->Inicial;
    
    lista->len--;
    return primero;
}

// EXTRAER LIFO (Pila): El último que entró es el primero que sale
Registro* extraer_lifo(Lista *lista) {
    if (lista->Final->prev == lista->Inicial) {
        return NULL; // La lista está vacía
    }
    
    Registro *ultimo = lista->Final->prev;
    
    // Desenlazamos el último nodo
    lista->Final->prev = ultimo->prev;
    ultimo->prev->next = lista->Final;
    
    lista->len--;
    return ultimo;
}

int main() {
    Lista cola;
    Lista pila;
    Registro *elemento;
    
    char nombre[50];
    char direccion[50];
    int edad;

    printf("========================================\n");
    printf("       First In First Out (FIFO)        \n");
    printf("========================================\n");
    crear_lista(&cola);

    // Se agregan los elementos a la cola
    strcpy(nombre, "Freddy");
    strcpy(direccion, "Argentina");
    agregar(&cola, 1, nombre, direccion, 47);

    strcpy(nombre, "Anabel");
    strcpy(direccion, "Oaxaca");
    agregar(&cola, 2, nombre, direccion, 45);

    strcpy(nombre, "Angel");
    strcpy(direccion, "CDMX");
    agregar(&cola, 3, nombre, direccion, 28);

    strcpy(nombre, "Fabian");
    strcpy(direccion, "Ticoman");
    agregar(&cola, 4, nombre, direccion, 23);

    strcpy(nombre, "Sofia");
    strcpy(direccion, "Chiapas");
    agregar(&cola, 5, nombre, direccion, 20);
    
    printf("Registros en Cola: %d\n\n", cola.len);

    while ((elemento = extraer_fifo(&cola)) != NULL) {
        printf("Elemento: %-2d | Nombre: %-10s | Dirección: %-12s | Edad: %2d años | Restantes: %d\n",  
               elemento->n_elemento, elemento->nombre, elemento->direccion, elemento->edad, cola.len);
        free(elemento); 
    }

    printf("\n======================================\n");
    printf("       Last In First Out (LIFO)         \n");
    printf("========================================\n");
    crear_lista(&pila);

    // Se agregan los elementos a la pila
    strcpy(nombre, "Freddy");
    strcpy(direccion, "Argentina");
    agregar(&pila, 1, nombre, direccion, 47);

    strcpy(nombre, "Anabel");
    strcpy(direccion, "Oaxaca");
    agregar(&pila, 2, nombre, direccion, 45);

    strcpy(nombre, "Angel");
    strcpy(direccion, "CDMX");
    agregar(&pila, 3, nombre, direccion, 28);

    strcpy(nombre, "Fabian");
    strcpy(direccion, "Ticoman");
    agregar(&pila, 4, nombre, direccion, 23);

    strcpy(nombre, "Sofia");
    strcpy(direccion, "Chiapas");
    agregar(&pila, 5, nombre, direccion, 20);
    
    printf("Registros en la pila: %d\n\n", pila.len);

    while ((elemento = extraer_lifo(&pila)) != NULL) {
        printf("Elemento: %-2d | Nombre: %-10s | Dirección: %-12s | Edad: %2d años | Restantes: %d\n",
               elemento->n_elemento, elemento->nombre, elemento->direccion, elemento->edad, pila.len);
        free(elemento); 
    }

    // Liberar los nodos creados
    free(cola.Inicial); free(cola.Final);
    free(pila.Inicial); free(pila.Final);

    return 0;
}
