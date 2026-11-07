#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>  // Para rand() y srand()
#include <ctime>    // Para time()
#include <algorithm>
#include <map>

using namespace std;

// Función para evaluar los dados en la categoría correspondiente
int evaluar_categoria(const vector<int>& dados, int categoria) {
    vector<int> frecuencias(7, 0); // Índices del 1 al 6 (el 0 se ignora)
    int suma_total = 0;
    
    // Contamos cuántas veces sale cada número y calculamos la suma total
    for (int dado : dados) {
        frecuencias[dado]++;
        suma_total += dado;
    }

    switch(categoria) {
        case 1: case 2: case 3: case 4: case 5: case 6:
            return frecuencias[categoria] * categoria;
            
        case 7: 
            return suma_total;
            
        case 8: 
            for (int i = 1; i <= 6; i++) {
                if (frecuencias[i] >= 3) return suma_total;
            }
            return 0;
            
        case 9: 
            for (int i = 1; i <= 6; i++) {
                if (frecuencias[i] >= 4) return suma_total;
            }
            return 0;
            
        case 10: 
            for (int i = 1; i <= 6; i++) {
                if (frecuencias[i] == 5) return 50;
            }
            return 0;
            
        case 11: 
            if ((frecuencias[1] && frecuencias[2] && frecuencias[3] && frecuencias[4]) ||
                (frecuencias[2] && frecuencias[3] && frecuencias[4] && frecuencias[5]) ||
                (frecuencias[3] && frecuencias[4] && frecuencias[5] && frecuencias[6])) {
                return 25;
            }
            return 0;
            
        case 12: 
            if ((frecuencias[1] && frecuencias[2] && frecuencias[3] && frecuencias[4] && frecuencias[5]) ||
                (frecuencias[2] && frecuencias[3] && frecuencias[4] && frecuencias[5] && frecuencias[6])) {
                return 35;
            }
            return 0;
            
        case 13: { 
            bool hay_tres = false, hay_dos = false;
            for (int i = 1; i <= 6; i++) {
                if (frecuencias[i] == 3) hay_tres = true;
                if (frecuencias[i] == 2) hay_dos = true;
            }
            if (hay_tres && hay_dos) return 40;
            return 0;
        }
    }
    return 0;
}

// Función que simula una partida completa de 13 rondas

int jugar_partida(ofstream& archivo) {
    vector<int> dados(5);
    int suma_primeras_6 = 0;
    int puntaje_partida = 0;

    for (int ronda = 1; ronda <= 13; ronda++) {
        // Tirar los 5 dados usando rand()
        for (int i = 0; i < 5; i++) {
            dados[i] = (rand() % 6) + 1; // Genera un número del 1 al 6
        }

        // Evaluar la puntuación de esta ronda
        int puntaje = evaluar_categoria(dados, ronda);
        
        // Sumar al total de la categoría 1 a la 6 para el bono
        if (ronda <= 6) {
            suma_primeras_6 += puntaje;
        }
        puntaje_partida += puntaje;

        // Guardar el registro en el archivo de texto
        archivo << "Ronda " << ronda << " - Dados: [";
        for (int i = 0; i < 5; i++) archivo << dados[i] << (i<4 ? "," : "");
        archivo << "] - Puntaje: " << puntaje << "\n";
    }

    // Calcular el bono de 35 puntos
    int bono = (suma_primeras_6 >= 63) ? 35 : 0;
    puntaje_partida += bono;

    archivo << "Bono: " << bono << " | Puntaje Total de la Partida: " << puntaje_partida << "\n";
    archivo << "---------------------------------------------------------\n";

    return puntaje_partida;
}

int main() {
    // Inicializar la semilla para los números aleatorios basados en el reloj del sistema
    srand(time(NULL));

    // Abrir archivo de texto para guardar los resultados
    ofstream archivo_salida("P1_Resultados.txt");
    if (!archivo_salida.is_open()) {
        cerr << "Error al crear el archivo." << endl;
        return 1;
    }

    int n_simulaciones = 1000;
    vector<int> historial_puntajes;

    // Ejecutar las simulaciones
    for (int i = 0; i < n_simulaciones; i++) {
        archivo_salida << "--- INICIO DE PARTIDA " << (i + 1) << " ---\n";
        int puntaje_final = jugar_partida(archivo_salida);
        historial_puntajes.push_back(puntaje_final);
    }

    archivo_salida.close();

    // Calcular máximo y mínimo usando
    int maximo = *max_element(historial_puntajes.begin(), historial_puntajes.end());
    int minimo = *min_element(historial_puntajes.begin(), historial_puntajes.end());

    // Calcular la moda (puntuación más común)
    map<int, int> conteo_puntajes;
    int moda = 0, max_repeticiones = 0;
    
    for (int puntaje : historial_puntajes) {
        conteo_puntajes[puntaje]++;
        if (conteo_puntajes[puntaje] > max_repeticiones) {
            max_repeticiones = conteo_puntajes[puntaje];
            moda = puntaje;
        }
    }

    // Imprimir los resultados en consola
    cout << "Simulacion terminada con " << n_simulaciones << " partidas." << endl;
    cout << "Los detalles se guardaron en 'P1_Resultados.txt'." << endl;
    cout << "Puntuacion maxima: " << maximo << endl;
    cout << "Puntuacion minima: " << minimo << endl;
    cout << "Puntuacion mas comun: " << moda << " (obtenida " << max_repeticiones << " veces)" << endl;

    return 0;
}
