#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// --- PARÁMETROS DEL ALGORITMO ---
#define TAM_POBLACION 50
#define NUM_GENERACIONES 100
#define PROB_CRUZA 0.90
#define PROB_MUTACION 0.01
#define PI 3.14159265358979323846

// --- ESTRUCTURA DEL INDIVIDUO ---
typedef struct {
    uint64_t cromosoma; 
    double aptitud;     
} Individuo;

// --- PROTOTIPOS DE FUNCIONES ---

uint64_t generar_rand64();
void inicializar_poblacion(Individuo poblacion[]);
double decodificar(uint32_t binario);
double calcular_rastrigin(double x, double y);
void evaluar_poblacion(Individuo poblacion[], int tamano);
void cruza(Individuo padre1, Individuo padre2, Individuo *hijo1, Individuo *hijo2);
void mutar(Individuo *ind);
int comparar_aptitud(const void *a, const void *b);
void seleccionar_sobrevivientes(Individuo poblacion_actual[], Individuo hijos[]);
void imprimir_binario(uint64_t cromosoma);
// --- FUNCIÓN PRINCIPAL ---
int main() {
    srand((unsigned int)time(NULL));

    Individuo poblacion[TAM_POBLACION];
    Individuo hijos[TAM_POBLACION];

    inicializar_poblacion(poblacion);
    evaluar_poblacion(poblacion, TAM_POBLACION);
    
    
    FILE *archivo = fopen("resultados.csv", "w");
    fprintf(archivo, "X,Y,Aptitud\n");
    
    
    for (int gen = 0; gen < NUM_GENERACIONES; gen++) {
        for (int i = 0; i < TAM_POBLACION; i += 2) {
            int p1 = rand() % TAM_POBLACION;
            int p2 = rand() % TAM_POBLACION;
            
            double r = (double)rand() / RAND_MAX;
            if (r < PROB_CRUZA) {
                cruza(poblacion[p1], poblacion[p2], &hijos[i], &hijos[i+1]);
            } else {
                hijos[i] = poblacion[p1];
                hijos[i+1] = poblacion[p2];
            }

            mutar(&hijos[i]);
            mutar(&hijos[i+1]);
        }

        evaluar_poblacion(hijos, TAM_POBLACION);
        seleccionar_sobrevivientes(poblacion, hijos);
        
        printf("Generacion %d | Mejor aptitud: %f\n", gen + 1, poblacion[0].aptitud);
        printf("Cromosoma: ");
        imprimir_binario(poblacion[0].cromosoma);
        printf("\n");
        
        
        uint32_t x_bin = (uint32_t)(poblacion[0].cromosoma >> 32);
        uint32_t y_bin = (uint32_t)(poblacion[0].cromosoma & 0xFFFFFFFF);
        double x_real = decodificar(x_bin);
        double y_real = decodificar(y_bin);

        fprintf(archivo, "%f,%f,%f\n", x_real, y_real, poblacion[0].aptitud);
    }
    fclose(archivo);
    return 0;
}

// --- DESARROLLO DE FUNCIONES ---

uint64_t generar_rand64() {
    uint64_t r1 = (uint64_t)rand() & 0xFFFF;
    uint64_t r2 = (uint64_t)rand() & 0xFFFF;
    uint64_t r3 = (uint64_t)rand() & 0xFFFF;
    uint64_t r4 = (uint64_t)rand() & 0xFFFF;
    return (r1 << 48) | (r2 << 32) | (r3 << 16) | r4;
}

void inicializar_poblacion(Individuo poblacion[]) {
    for (int i = 0; i < TAM_POBLACION; i++) {
        poblacion[i].cromosoma = generar_rand64();
        poblacion[i].aptitud = 0.0; 
    }
}

double decodificar(uint32_t binario) {
    double limite_inferior = -5.12; 
    double limite_superior = 5.12;
    double denominador = (double)UINT32_MAX; 
    
    double proporcion = (double)binario / denominador;
    return limite_inferior + (proporcion * (limite_superior - limite_inferior));
}

double calcular_rastrigin(double x, double y) {
    double A = 10.0;
    double termino_x = (x * x) - A * cos(2.0 * PI * x);
    double termino_y = (y * y) - A * cos(2.0 * PI * y);
    return 20.0 + termino_x + termino_y;
}

void evaluar_poblacion(Individuo poblacion[], int tamano) {
    for (int i = 0; i < tamano; i++) {
        uint32_t x_bin = (uint32_t)(poblacion[i].cromosoma >> 32);
        uint32_t y_bin = (uint32_t)(poblacion[i].cromosoma & 0xFFFFFFFF);
        
        double x_real = decodificar(x_bin);
        double y_real = decodificar(y_bin);
        
        poblacion[i].aptitud = calcular_rastrigin(x_real, y_real);
    }
}

void cruza(Individuo padre1, Individuo padre2, Individuo *hijo1, Individuo *hijo2) {
    uint64_t mascara_alta = 0xFFFFFFFF00000000ULL; 
    uint64_t mascara_baja = ~mascara_alta; 

    hijo1->cromosoma = (padre1.cromosoma & mascara_alta) | (padre2.cromosoma & mascara_baja);
    hijo2->cromosoma = (padre2.cromosoma & mascara_alta) | (padre1.cromosoma & mascara_baja);
}

void mutar(Individuo *ind) {
    for (int i = 0; i < 64; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < PROB_MUTACION) {
            uint64_t mascara_bit = 1ULL << i;
            ind->cromosoma = ind->cromosoma ^ mascara_bit;
        }
    }
}

int comparar_aptitud(const void *a, const void *b) {
    Individuo *indA = (Individuo *)a;
    Individuo *indB = (Individuo *)b;
    
    if (indA->aptitud < indB->aptitud) return -1;
    else if (indA->aptitud > indB->aptitud) return 1;
    else return 0;
}

void seleccionar_sobrevivientes(Individuo poblacion_actual[], Individuo hijos[]) {
    Individuo poblacion_unida[TAM_POBLACION * 2];
    
    for (int i = 0; i < TAM_POBLACION; i++) {
        poblacion_unida[i] = poblacion_actual[i];
        poblacion_unida[i + TAM_POBLACION] = hijos[i];
    }
    
    qsort(poblacion_unida, TAM_POBLACION * 2, sizeof(Individuo), comparar_aptitud);
    
    for (int i = 0; i < TAM_POBLACION; i++) {
        poblacion_actual[i] = poblacion_unida[i];
    }
}

void imprimir_binario(uint64_t cromosoma) {
    for (int i = 63; i >= 0; i--) {
        // Extraer el bit en la posición 'i'
        int bit = (cromosoma >> i) & 1;
        printf("%d", bit);
        
        // Imprimir una barra separadora exactamente a la mitad (entre X e Y)
        if (i == 32) {
            printf(" | ");
        } 
        // Imprimir un espacio cada 8 bits 
        else if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}
