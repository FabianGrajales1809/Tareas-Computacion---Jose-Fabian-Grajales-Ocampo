import random
from collections import Counter

# Función para evaluar los dados en la categoría correspondiente
def evaluar_categoria(dados, categoria):
    frecuencias = Counter(dados) 
    suma_total = sum(dados)
    
    if 1 <= categoria <= 6:
        # Extrae la frecuencia del número (o 0 si no salió) y lo multiplica
        return frecuencias.get(categoria, 0) * categoria
        
    elif categoria == 7:
        return suma_total
        
    elif categoria == 8:
        # Tres de un tipo
        if any(v >= 3 for v in frecuencias.values()):
            return suma_total
        return 0
        
    elif categoria == 9:
        # Cuatro de un tipo
        if any(v >= 4 for v in frecuencias.values()):
            return suma_total
        return 0
        
    elif categoria == 10:
        # Cinco de un tipo
        if any(v == 5 for v in frecuencias.values()):
            return 50
        return 0
        
    elif categoria == 11:
        # Secuencia corta (4 consecutivos) usando subconjuntos
        dados_unicos = set(dados)
        if ({1, 2, 3, 4}.issubset(dados_unicos) or 
            {2, 3, 4, 5}.issubset(dados_unicos) or 
            {3, 4, 5, 6}.issubset(dados_unicos)):
            return 25
        return 0
        
    elif categoria == 12:
        # Secuencia larga (5 consecutivos)
        dados_unicos = set(dados)
        if ({1, 2, 3, 4, 5}.issubset(dados_unicos) or 
            {2, 3, 4, 5, 6}.issubset(dados_unicos)):
            return 35
        return 0
        
    elif categoria == 13:
        # Full house: un trío y un par
        valores = frecuencias.values()
        if 3 in valores and 2 in valores:
            return 40
        return 0
        
    return 0

# Función que simula una partida completa de 13 rondas
def jugar_partida(archivo):
    suma_primeras_6 = 0
    puntaje_partida = 0
    
    for ronda in range(1, 14):
        # Generar 5 dados aleatorios del 1 al 6
        dados = [random.randint(1, 6) for _ in range(5)]
        puntaje = evaluar_categoria(dados, ronda)
        
        # Sumar al total de la categoría 1 a la 6 para el bono
        if ronda <= 6:
            suma_primeras_6 += puntaje
            
        puntaje_partida += puntaje
        
        # Escribir en el archivo
        archivo.write(f"Ronda {ronda} - Dados: {dados} - Puntaje: {puntaje}\n")
        
    # Calcular el bono de 35 puntos
    bono = 35 if suma_primeras_6 >= 63 else 0
    puntaje_partida += bono
    
    archivo.write(f"Bono: {bono} | Puntaje Total de la Partida: {puntaje_partida}\n")
    archivo.write("-" * 57 + "\n")
    
    return puntaje_partida

def main():
    n_simulaciones = 1000
    historial_puntajes = []
    
    # Abrir archivo para escritura
    with open("P1_Resultados_Python.txt", "w") as archivo_salida:
        for i in range(n_simulaciones):
            archivo_salida.write(f"--- INICIO DE PARTIDA {i + 1} ---\n")
            puntaje_final = jugar_partida(archivo_salida)
            historial_puntajes.append(puntaje_final)
            
    # Calcular estadísticas utilizando las funciones integradas de Python
    maximo = max(historial_puntajes)
    minimo = min(historial_puntajes)
    
    # most_common(1) devuelve una lista con una tupla del valor más repetido y su frecuencia: [(moda, repeticiones)]
    moda_info = Counter(historial_puntajes).most_common(1)[0]
    
    # Imprimir resultados en la terminal
    print(f"Simulacion terminada con {n_simulaciones} partidas.")
    print("Los detalles se guardaron en 'P1_Resultados_Python.txt'.")
    print(f"Puntuacion maxima: {maximo}")
    print(f"Puntuacion minima: {minimo}")
    print(f"Puntuacion mas comun (Moda): {moda_info[0]} (obtenida {moda_info[1]} veces)")

# Punto de entrada estándar en Python
if __name__ == "__main__":
    main()
