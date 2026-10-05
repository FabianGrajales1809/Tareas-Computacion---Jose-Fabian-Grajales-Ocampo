 #include <time.h>
#include <memory>
#include <iostream>
#include <chrono>

extern "C"

{

#include <immintrin.h>

}


using namespace std;


// COUNTER TYPE //
typedef unsigned long long bench_t;
static bench_t before;
static bench_t after;


// ASKS FOR THE TIME STAMP COUNTER (= WHAT TIME IS IT PLEASE ?)//

static inline bench_t cycles(void) {
    unsigned int hi, lo;
    __asm__ __volatile__ ("rdtsc\n\t":"=a" (lo), "=d"(hi));
    return ((bench_t) lo) | (((bench_t) hi) << 32);
}

// Versión normal en flotantes simples
float horner(float X, float *coef, long size){
    float ACC = 0.0f;
    int i;
    for(i=0; i<size; i++){
        ACC = (ACC + coef[i]) * X;
    }
    return ACC;    
}

// Versión Intrinsics en flotantes simples
float horner_intrinsic(float X, float *coef, long size){
    float *R, P;
    int i;
    // __m256 para float (8 elementos) en vez de __m256d (4 elementos)
    __m256 *ymm0, X256, Y;    
    ymm0 = (__m256*)coef; 
    
    // Al procesar 8 elementos a la vez, X debe estar elevado a la 8
    float X2 = X * X;
    float X4 = X2 * X2;
    float X8 = X4 * X4;
    
    X256 = _mm256_set1_ps(X8);
    Y = _mm256_set1_ps(0.0f);
    // El tamaño de bloque es 8, así que el límite es size/8 - 1
    for(i=0; i<size/8-1; i++){
        Y = _mm256_add_ps(Y, ymm0[i]);
        Y = _mm256_mul_ps(Y, X256);
    }
    Y = _mm256_add_ps(Y, ymm0[i]);
    R = (float *)(&Y);
    // Potencias faltantes para la reconstrucción final
    float X3 = X2 * X;
    float X5 = X4 * X;
    float X6 = X4 * X2;
    float X7 = X4 * X3;
    // Se reconstruye el polinomio sumando los 8 resultados paralelos
    P  = R[7] * X;
    P += R[6] * X2;
    P += R[5] * X3;
    P += R[4] * X4;
    P += R[3] * X5;
    P += R[2] * X6;
    P += R[1] * X7;
    P += R[0] * X8;
    
    return P;
}


int main(){
    alignas(32) float coef[4000];
    float X = 1.1f;
    float R;
    int i, num_trails = 100000;
    srand(time(NULL));    
    float *coeficientes;
    int j;
    coeficientes = (float *)_mm_malloc(10000 * sizeof(float), 32);
    
    for(j=0; j<1; j++){
        for(i=0; i<10000; i++){
            coeficientes[i] = (float)(rand()%1000) / 1000.0f;
            if(i<10)
                cout << coeficientes[i] << endl;
        }
        R = horner(X, coeficientes, 10000);
        cout << "Resultado (Normal): " << R << endl;
        R = horner_intrinsic(X, coeficientes, 10000);
        cout << "Resultado (Intrinsics): " << R << endl;        
    }
    cout << "\n--- Midiendo ---" << endl;
    // --- MEDICIÓN NORMAL ---
    auto start_normal = std::chrono::high_resolution_clock::now(); // Inicia cronómetro
    before = cycles();                                             // Inicia contador de ciclos

    for(j=0; j<num_trails; j++)    
        R = horner(X, coeficientes, 10000);

    after = cycles();                                              // Detiene contador de ciclos
    auto end_normal = std::chrono::high_resolution_clock::now();   // Detiene cronómetro

    // Calcula la diferencia en milisegundos
    std::chrono::duration<double, std::milli> ms_normal = end_normal - start_normal;

    cout << "NORMAL:" << endl;
    cout << " - Ciclos: " << (after - before) << endl;
    cout << " - Tiempo: " << ms_normal.count() << " ms\n" << endl;

    // --- MEDICIÓN INTRINSICS ---
    auto start_intr = std::chrono::high_resolution_clock::now();
    before = cycles();
    for(j=0; j<num_trails; j++)    
        R = horner_intrinsic(X, coeficientes, 10000);
    
    after = cycles();
    auto end_intr = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_intr = end_intr - start_intr;


    cout << "INTRINSICS (AVX):" << endl;
    cout << " - Ciclos: " << (after - before) << endl;
    cout << " - Tiempo: " << ms_intr.count() << " ms\n" << endl;

    _mm_free(coeficientes);
    
    return 0;

}
