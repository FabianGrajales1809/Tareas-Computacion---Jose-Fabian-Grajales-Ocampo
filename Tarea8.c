// Incluye las definiciones de los intrinsics de Intel (AVX, AVX2, FMA)
#include <immintrin.h>

// Función exportada: recibe dos punteros constantes a float (a y b) y su tamaño (n)
float producto_punto(const float* a, const float* b, int n) {
    // Crea un registro vectorial de 256 bits (caben 8 floats de 32 bits)
    // y lo inicializa en [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    __m256 vsuma = _mm256_setzero_ps();

    // Índice declarado fuera del ciclo para reutilizarlo si sobran elementos al final
    int i = 0;

    // Avanza de 8 en 8 mientras queden al menos 8 datos por leer en los arreglos
    for (; i <= n - 8; i += 8) {
        // Carga 8 floats consecutivos desde la RAM (&a[i] ... &a[i+7]) al registro va.
        // La 'u' (unaligned) permite leer sin exigir alineación estricta de 32 bytes.
        __m256 va = _mm256_loadu_ps(&a[i]);

        // Carga los siguientes 8 floats consecutivos de 'b' al registro vb
        __m256 vb = _mm256_loadu_ps(&b[i]);

        // Fused Multiply-Add (FMA): multiplica los 8 pares (va * vb) y suma cada
        // producto al carril correspondiente de 'vsuma' en una sola instrucción de CPU.
        vsuma = _mm256_fmadd_ps(va, vb, vsuma);
    }

    // Arreglo tradicional en memoria RAM para extraer los 8 acumuladores del registro
    float parciales[8];

    // Copia los 8 floats del registro vectorial 'vsuma' hacia el arreglo 'parciales'
    _mm256_storeu_ps(parciales, vsuma);

    // Suma los 8 acumuladores parciales para reducirlos a un solo número escalar
    float total = 0.0f;
    for (int j = 0; j < 8; j++) {
        total += parciales[j];
    }

    // Cola escalar: si 'n' no fue múltiplo de 8 (ej. n = 10), aquí se procesan
    // uno por uno los elementos sobrantes (índices 8 y 9)
    for (; i < n; i++) {
        total += a[i] * b[i];
    }

    // Devuelve el resultado final a quien haya llamado la función (Python)
    return total;
}
