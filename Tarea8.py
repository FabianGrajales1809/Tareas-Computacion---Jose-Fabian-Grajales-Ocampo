import ctypes   # Permite cargar bibliotecas en C y traducir tipos de datos
import math     # Para comparar números flotantes con margen de tolerancia
import random   # Para generar datos de prueba
import time     # Para medir tiempos de ejecución con alta precisión

#Carga en memoria el archivo compilado (.so) desde la carpeta actual
lib = ctypes.CDLL("./Tarea8.so")

#Define un alias equivalente al tipo 'float*' de C (puntero a float de 32 bits)
FLOAT_PTR = ctypes.POINTER(ctypes.c_float)

# Especifica exactamente qué tipos de parámetros espera recibir la función en C.
# Si no se define, ctypes puede enviar tipos incompatibles y causar fallos de memoria.
lib.producto_punto.argtypes = [FLOAT_PTR, FLOAT_PTR, ctypes.c_int]

# Indica que la función en C retorna un 'float' de 32 bits
# (por defecto ctypes asume que toda función en C devuelve un 'int').
lib.producto_punto.restype = ctypes.c_float

# Configura el tamaño de prueba y fija una semilla para repetir los mismos datos
N = 10_000
random.seed(42)

# Crea dos listas normales de Python con 10,000 números flotantes
lista_a = [random.random() for _ in range(N)]
lista_b = [random.random() for _ in range(N)]

# Define un tipo de dato de C equivalente a un arreglo contiguo: float[10000]
ArregloC = ctypes.c_float * N

# Desempaqueta (*) cada lista de Python y copia sus valores a memoria contigua de C
arr_a = ArregloC(*lista_a)
arr_b = ArregloC(*lista_b)

#Toma el tiempo, ejecuta la función compilada en C y guarda el resultado
t0 = time.perf_counter()
res_c = lib.producto_punto(arr_a, arr_b, N)
t_c = time.perf_counter() - t0

#Calcula el mismo producto punto en Python puro para comprobar que está bien
t0 = time.perf_counter()
res_py = sum(x * y for x, y in zip(lista_a, lista_b))
t_py = time.perf_counter() - t0

# Imprime resultados y tiempos en milisegundos
print(f"Resultado C (AVX2): {res_c:.6f}  | Tiempo: {t_c * 1000:.4f} ms")
print(f"Resultado Python:   {res_py:.6f}  | Tiempo: {t_py * 1000:.4f} ms")


