import matplotlib.pyplot as plt
from collections import Counter
import re

def calcular_histograma(ruta_archivo):
    """Lee un archivo y devuelve una tabla hash (Counter) con las frecuencias de las letras."""
    try:
        with open(ruta_archivo, 'r', encoding='utf-8') as f:
            texto = f.read().lower()
            
            # Filtrar para conservar solo letras del alfabeto español
            texto_filtrado = re.sub(r'[^a-zñ]', '', texto)
            
            # Counter funciona como la tabla hash requerida para contar frecuencias
            return Counter(texto_filtrado)
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo '{ruta_archivo}'.")
        return Counter()

def graficar_histograma(frecuencias, titulo, color):
    """Grafica el histograma ordenado de mayor a menor frecuencia."""
    if not frecuencias:
        return
        
    # Ordenar de mayor a menor frecuencia para visualizar los picos
    letras_ordenadas = [item[0] for item in frecuencias.most_common()]
    conteos = [item[1] for item in frecuencias.most_common()]

    plt.figure(figsize=(10, 5))
    plt.bar(letras_ordenadas, conteos, color=color, edgecolor='black')
    plt.title(titulo)
    plt.xlabel("Letras")
    plt.ylabel("Frecuencia (Apariciones)")
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.show()

def main():
    print("--- ANÁLISIS DE FRECUENCIAS ---")
    
    # 1. Calcular histogramas utilizando contenedor Hash[cite: 3]
    freq_original = calcular_histograma("texto_original.txt")
    freq_cifrado = calcular_histograma("texto_cifrado.txt")
    
    if not freq_original or not freq_cifrado:
        return

    # Mostrar las letras más comunes en la consola
    top_5_orig = freq_original.most_common(5)
    top_5_cif = freq_cifrado.most_common(5)
    
    print("\nTop 5 letras más frecuentes en el texto ORIGINAL:")
    for letra, count in top_5_orig:
        print(f"  {letra}: {count} veces")
        
    print("\nTop 5 letras más frecuentes en el texto CIFRADO:")
    for letra, count in top_5_cif:
        print(f"  {letra}: {count} veces")
        
    print("\n¿Es posible descifrarlo? Sí.")
    print("Al comparar los histogramas (las letras con los picos más altos),")
    print(f"se puede deducir que la letra '{top_5_cif[0][0]}' del texto cifrado ")
    print(f"corresponde a la '{top_5_orig[0][0]}' del original, y así sucesivamente.")
    
    # 2. Graficar ambos histogramas para el reporte[cite: 3]
    graficar_histograma(freq_original, "Histograma de Frecuencias - Texto Original", 'skyblue')
    graficar_histograma(freq_cifrado, "Histograma de Frecuencias - Texto Cifrado", 'salmon')

if __name__ == "__main__":
    main()
