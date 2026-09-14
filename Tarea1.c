#include <stdio.h>
#include <immintrin.h> 

union Data128 {
    __m128i vec;                    
    // partes64[0]=Baja, partes64[1]=Alta
    unsigned long long partes64[2]; 
    unsigned char bytes[16];        
};

int main() {
    union Data128 A, B, Resultado;

    // Inicializamos las partes altas y bajas
    A.partes64[1] = 0x0000000000000001ULL; 
    A.partes64[0] = 0x0000000000000005ULL; 

    B.partes64[1] = 0x0000000000000002ULL; 
    B.partes64[0] = 0x000000000000000AULL; 

    // Números de entrada
    printf("Numero A (Hexadecimal): 0x%016llX%016llX\n", A.partes64[1], A.partes64[0]);
    printf("Numero B (Hexadecimal): 0x%016llX%016llX\n\n", B.partes64[1], B.partes64[0]);
    
    
    unsigned long long a = A.partes64[0];
    unsigned long long b = B.partes64[0];
    unsigned long long hi;
    unsigned long long dst; 

    // Multiplicación de las partes bajas
    dst = _mulx_u64(a, b, &hi);
    
    // Multiplicación cruzada
    unsigned long long ahi_b = A.partes64[1] * b;
    unsigned long long a_bhi = a * B.partes64[1];
    
    // Resultado de 128 bits
    Resultado.partes64[0] = dst;
    Resultado.partes64[1] = hi + ahi_b + a_bhi;

    printf("Resultado Hexadecimal: 0x%016llX%016llX\n\n", Resultado.partes64[1], Resultado.partes64[0]);

    for(int i = 0; i < 16; i++) {
        // Imprimimos usando la vista de bytes de la variable Resultado
        printf("[%02d] ( %p) = %02X\n", i, (void*)&Resultado.bytes[i], Resultado.bytes[i]);
    }

    return 0;
}
