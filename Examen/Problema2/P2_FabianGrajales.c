#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

#define TAM 27

// Alfabeto original incluyendo la ñ
const wchar_t ALFABETO[TAM+1] = L"abcdefghijklmnñopqrstuvwxyz";

// Función para buscar el índice de un caracter en un alfabeto dado
int buscar_indice(wchar_t c, const wchar_t *alfabeto) {
    for (int i = 0; i < TAM; i++) {
        if (alfabeto[i] == c) return i;
    }
    return -1;
}

// Genera una permutación aleatoria y la guarda en un archivo
void generar_y_guardar_clave(const char *nombre_archivo, wchar_t *clave) {
    wcscpy(clave, ALFABETO);
    
    // Algoritmo para desordenar el alfabeto
    for (int i = TAM - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        wchar_t temp = clave[i];
        clave[i] = clave[j];
        clave[j] = temp;
    }

    FILE *archivo = fopen(nombre_archivo, "w, ccs=UTF-8");
    if (archivo) {
        fwprintf(archivo, L"%ls", clave);
        fclose(archivo);
        wprintf(L"Clave de permutación generada y guardada en '%s'.\n", nombre_archivo);
    }
}

// Cifra o descifra un archivo dependiendo de los alfabetos de origen y destino
void procesar_archivo(const char *archivo_entrada, const char *archivo_salida, 
                      const wchar_t *origen, const wchar_t *destino) {
    FILE *entrada = fopen(archivo_entrada, "r, ccs=UTF-8");
    FILE *salida = fopen(archivo_salida, "w, ccs=UTF-8");

    if (!entrada || !salida) {
        wprintf(L"Error al abrir los archivos de texto.\n");
        return;
    }

    wint_t c;
    while ((c = fgetwc(entrada)) != WEOF) {
        wchar_t c_min = towlower(c); // Convertir a minúscula para estandarizar
        int indice = buscar_indice(c_min, origen);

        if (indice != -1) {
            // Si es una letra del alfabeto, la sustituimos
            fputwc(destino[indice], salida);
        } else {
            // Si es espacio, puntuación o salto de línea, se queda igual
            fputwc(c, salida);
        }
    }

    fclose(entrada);
    fclose(salida);
}

int main() {
    // Configurar la localización para soportar caracteres UTF-8
    setlocale(LC_ALL, "");
    srand(time(NULL));

    wchar_t clave_permutada[TAM + 1];
    
    // Archivos de prueba debemos tener "texto_original.txt" ya creado
    const char *arch_original = "texto_original.txt";
    const char *arch_cifrado = "texto_cifrado.txt";
    const char *arch_descifrado = "texto_descifrado.txt";
    const char *arch_clave = "clave.txt";

    wprintf(L"--- SISTEMA DE CIFRADO POR SUSTITUCIÓN ---\n");

    // Generar la permutación
    generar_y_guardar_clave(arch_clave, clave_permutada);
    wprintf(L"Alfabeto permutado: %ls\n", clave_permutada);

    // Cifrar el mensaje
    procesar_archivo(arch_original, arch_cifrado, ALFABETO, clave_permutada);
    wprintf(L"Texto cifrado guardado en '%s'.\n", arch_cifrado);

    // Descifrar el mensaje
    // Invertimos el orden: buscamos en la clave y escribimos desde el alfabeto original
    procesar_archivo(arch_cifrado, arch_descifrado, clave_permutada, ALFABETO);
    wprintf(L"Texto descifrado recuperado en '%s'.\n", arch_descifrado);

    return 0;
}
