#include <stdio.h>
#include <immintrin.h> 

union Data128 {
    __m128i vec;                    
    unsigned long long partes64[2]; // [0]=Baja, [1]=Alta
    unsigned char bytes[16];        
};

// Alamacenar el resultado de 256 bits
union Data256 {
    __m256i vec;
    unsigned long long partes64[4]; 
    unsigned char bytes[32];
};

int main() {
    union Data128 A, B;
    union Data256 Resultado = {0}; 

    // Inicializamos las partes altas y bajas
    A.partes64[1] = 0x0000000000000001ULL; 
    A.partes64[0] = 0x0000000000000005ULL; 

    B.partes64[1] = 0x0000000000000002ULL; 
    B.partes64[0] = 0x000000000000000AULL; 

    // Números de entrada
    printf("Numero A (Hexadecimal): 0x%016llX%016llX\n", A.partes64[1], A.partes64[0]);
    printf("Numero B (Hexadecimal): 0x%016llX%016llX\n\n", B.partes64[1], B.partes64[0]);
    
    unsigned long long a_lo = A.partes64[0];
    unsigned long long a_hi = A.partes64[1];
    unsigned long long b_lo = B.partes64[0];
    unsigned long long b_hi = B.partes64[1];

    unsigned long long hi, lo;
    unsigned char c = 0; // Almacena el bit de acarreo (carry)

    // Producto parcial
    lo = _mulx_u64(a_lo, b_lo, &hi);
    Resultado.partes64[0] = lo;
    Resultado.partes64[1] = hi;

    // Producto parcial cruzado
    lo = _mulx_u64(a_lo, b_hi, &hi);
    c = _addcarry_u64(0, Resultado.partes64[1], lo, &Resultado.partes64[1]);
    c = _addcarry_u64(c, 0, hi, &Resultado.partes64[2]); 

    // Producto parcial cruzado: a_hi * b_lo (Usando partes64[1], partes64[2] y partes64[3])
    lo = _mulx_u64(a_hi, b_lo, &hi);
    c = _addcarry_u64(0, Resultado.partes64[1], lo, &Resultado.partes64[1]);
    c = _addcarry_u64(c, Resultado.partes64[2], hi, &Resultado.partes64[2]);
    c = _addcarry_u64(c, 0, 0, &Resultado.partes64[3]); 

    // Producto parcial: a_hi * b_hi (Usando partes64[2] y partes64[3])
    lo = _mulx_u64(a_hi, b_hi, &hi);
    c = _addcarry_u64(0, Resultado.partes64[2], lo, &Resultado.partes64[2]);
    c = _addcarry_u64(c, Resultado.partes64[3], hi, &Resultado.partes64[3]);

    // Resultado final de 256 bits
    printf("Resultado Hexadecimal (256 bits):\n0x%016llX %016llX %016llX %016llX\n\n", 
            Resultado.partes64[3], Resultado.partes64[2], 
            Resultado.partes64[1], Resultado.partes64[0]);

    for(int i = 0; i < 32; i++) {
        // Imprimimos usando la vista de bytes de la variable Resultado
        printf("[%02d] ( %p) = %02X\n", i, (void*)&Resultado.bytes[i], Resultado.bytes[i]);
    }

    return 0;
}
