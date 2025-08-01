#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <fstream>
#include <memory>
#include "estructuras_grandes.h"
#include "grafo_grande.h"
#include "metricas.h"

using namespace std;
using namespace chrono;

// Función para generar puntos de prueba aleatorios
vector<pair<int, int>> generar_puntos_prueba(int cantidad, int max_nodos) {
    vector<pair<int, int>> puntos;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, max_nodos - 1);
    
    for (int i = 0; i < cantidad; ++i) {
        int origen = dis(gen);
        int destino = dis(gen);
        while (destino == origen) {
            destino = dis(gen);
        }
        puntos.push_back({origen, destino});
    }
    return puntos;
}

// Función para ejecutar pruebas en paralelo
void ejecutar_pruebas_paralelas(const vector<pair<int, int>>& puntos_prueba,
                                vector<PruebaRendimiento>& resultados,
                                int thread_id, int inicio, int fin) {
    
    for (int i = inicio; i < fin; ++i) {
        int origen = puntos_prueba[i].first;
        int destino = puntos_prueba[i].second;
        
        // Probar cada algoritmo
        vector<string> algoritmos = {"BFS", "DFS", "BestFirst", "Dijkstra", "AStar"};
        
        for (const string& algo : algoritmos) {
            PruebaRendimiento prueba;
            prueba.origen = origen;
            prueba.destino = destino;
            prueba.algoritmo = algo;
            
            int camino[MAX_NODES_LARGE];
            int largo = 0;
            
            auto inicio_tiempo = high_resolution_clock::now();
            double memoria_inicial = obtener_memoria_actual();
            
            // Ejecutar algoritmo correspondiente
            if (algo == "BFS") {
                buscar_BFS_grande(origen, destino, camino, largo);
            } else if (algo == "DFS") {
                buscar_DFS_grande(origen, destino, camino, largo);
            } else if (algo == "BestFirst") {
                buscar_BestFirst_grande(origen, destino, camino, largo);
            } else if (algo == "Dijkstra") {
                buscar_Dijkstra_grande(origen, destino, camino, largo);
            } else if (algo == "AStar") {
                buscar_AStar_grande(origen, destino, camino, largo);
            }
            
            auto fin_tiempo = high_resolution_clock::now();
            double memoria_final = obtener_memoria_actual();
            
            prueba.tiempo_ms = duration_cast<microseconds>(fin_tiempo - inicio_tiempo).count() / 1000.0;
            prueba.longitud_camino = largo;
            prueba.memoria_mb = memoria_final - memoria_inicial;
            prueba.encontro_camino = (largo > 0);
            
            resultados[i * algoritmos.size() + (&algo - &algoritmos[0])] = prueba;
        }
        
        // Mostrar progreso
        if ((i - inicio) % 10 == 0) {
            cout << "Thread " << thread_id << ": " << (i - inicio) << "/" << (fin - inicio) << " completadas\n";
        }
    }
}

int main() {
    cout << "=== PROYECTO RUTAS PARTE II: GRAFOS GRANDES ===" << endl;
    cout << "Iniciando pruebas de rendimiento..." << endl;
    
    // Configuración
    const int NUM_PRUEBAS = 100;
    const int NUM_THREADS = thread::hardware_concurrency();
    
    cout << "Threads disponibles: " << NUM_THREADS << endl;
    cout << "Numero de pruebas: " << NUM_PRUEBAS << endl;
    
    // Generar grafo grande
    cout << "\n1. Generando grafo grande..." << endl;
    auto inicio_construccion = high_resolution_clock::now();
    
    if (!generar_grafo_grande()) {
        cerr << "Error al generar el grafo grande" << endl;
        return 1;
    }
    
    auto fin_construccion = high_resolution_clock::now();
    double tiempo_construccion = duration_cast<milliseconds>(fin_construccion - inicio_construccion).count();
    
    cout << "Grafo generado exitosamente!" << endl;
    cout << "Tiempo de construccion: " << tiempo_construccion << " ms" << endl;
    cout << "Nodos: " << MAX_NODES_LARGE << endl;
    cout << "Aristas aproximadas: " << contar_aristas_grandes() << endl;
    
    // Generar puntos de prueba
    cout << "\n2. Generando puntos de prueba..." << endl;
    vector<pair<int, int>> puntos_prueba = generar_puntos_prueba(NUM_PRUEBAS, MAX_NODES_LARGE);
    
    // Preparar resultados
    vector<PruebaRendimiento> resultados(NUM_PRUEBAS * 5); // 5 algoritmos
    
    // Ejecutar pruebas en paralelo
    cout << "\n3. Ejecutando pruebas en paralelo..." << endl;
    auto inicio_pruebas = high_resolution_clock::now();
    
    vector<thread> threads;
    int pruebas_por_thread = NUM_PRUEBAS / NUM_THREADS;
    
    for (int t = 0; t < NUM_THREADS; ++t) {
        int inicio = t * pruebas_por_thread;
        int fin = (t == NUM_THREADS - 1) ? NUM_PRUEBAS : (t + 1) * pruebas_por_thread;
        
        threads.emplace_back(ejecutar_pruebas_paralelas, 
                           ref(puntos_prueba), ref(resultados), t, inicio, fin);
    }
    
    // Esperar a que terminen todos los threads
    for (auto& t : threads) {
        t.join();
    }
    
    auto fin_pruebas = high_resolution_clock::now();
    double tiempo_total_pruebas = duration_cast<milliseconds>(fin_pruebas - inicio_pruebas).count();
    
    cout << "\nPruebas completadas en: " << tiempo_total_pruebas << " ms" << endl;
    
    // Analizar y mostrar resultados
    cout << "\n4. Analizando resultados..." << endl;
    analizar_resultados(resultados, NUM_PRUEBAS);
    
    // Guardar resultados en archivo
    guardar_resultados_csv(resultados, "resultados_parte2.csv");
    
    cout << "\n=== PRUEBAS COMPLETADAS ===" << endl;
    cout << "Resultados guardados en: resultados_parte2.csv" << endl;
    
    return 0;
}
