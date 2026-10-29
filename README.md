# JOSÉ FABIÁN GRAJALES OCAMPO

Este es el repositorio donde subiré las tareas del curso Computación
impartido por el Dr. Cuauhtemoc Mancillas López.


## Descripción
Tarea 1: Este programa consiste en la multiplicación de 2 números de 128 bits
usando la función _mulx_u64 de intel intrinsics guide con la biblioteca #include <immintrin.h> 

Tarea 2: Este programa utiliza una lista enlazada para implementar el funcionamiento de una FIFO (Cola)
(el primero en entrar es el primero en salir) y una LIFO (Pila) (el último en entrar es el primero en salir) 
almacenando datos básicos de personas.


Tarea 3: Este programa evalúa un polinomio utilizando el Método de Horner. 
Compara el rendimiento de una versión secuencial tradicional contra una versión vectorizada 
utilizando Intel Intrinsics (AVX) adaptada para variables de punto flotante simple (procesando 8 elementos a la vez).

Tarea 4: Este programa implementa una estructura de datos de Tabla Hash utilizando el método de encadenamiento separado 
mediante listas enlazadas para gestionar las colisiones.

Tarea 5: Este programa implementa un Algoritmo Genético (Evolutivo) utilizando operaciones a nivel de bits sobre enteros de 64 bits para encontrar el mínimo global de la función de Rastrigin de dos variables.

Tarea 6: Este programa implementa el algoritmo de compresión de Huffman en Python. Está dividido en dos scripts independientes (uno para comprimir y otro para descomprimir) e implementa un formato de archivo propio que guarda la tabla de frecuencias en una cabecera (header) usando JSON para reconstruir el árbol de manera autónoma.

## Compilación

    *Tarea 1:
        gcc -o Tarea1.o Tarea1.c -mbmi2
        ./Tarea1.o
    *Tarea 2:
        gcc -o Tarea2.o Tarea2.c
        ./Tarea2.o
    *Tarea 3:
        g++ Tarea3.cpp -O3 -mavx -mavx2 -o Tarea3
        ./Tarea3
     *Tarea 4:
        gcc -o Tarea4.o Tarea4.c
        ./Tarea4.o 
    *Tarea 5:
        gcc Tarea5.c -o Tarea5 -lm
        ./Tarea5
    *Tarea 6:
        Tarea6_comprimir.py
        Tarea6_descomprimir.py
