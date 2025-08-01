#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <fstream>
#include <memory>
#include <mutex>
#include "estructuras_grandes.h"
#include "grafo_grande.h"
#include "metricas.h"

using namespace std;
using namespace chrono;

// Mutex para sincronización
mutex resultados_mutex;
mutex cout_mutex;

// Función para imprimir de forma segura
void print_safe(const string& mensaje) {
    lock_guard<mutex> lock(cout_mutex);
    cout << mensaje << endl;
}

// Función para ejecutar pruebas en paralelo (versión corregida)
void ejecutar_pruebas_paralelas_seguras(const vector<pair<int, int>>& puntos_prueba,
                                       vector<PruebaRendimiento>& resultados,
                                       int thread_id, int inicio, int fin) {
    
    vector<PruebaRendimiento> resultados_locales; // Usar vector local para evitar problemas de concurrencia
    
    vector<string> algoritmos = {"BFS", "DFS", "BestFirst", "Dijkstra", "AStar"};
    
    for (int i = inicio; i < fin; ++i) {
        int origen = puntos_prueba[i].first;
        int destino = puntos_prueba[i].second;
        
        for (size_t a = 0; a < algoritmos.size(); ++a) {
            const string& algo = algoritmos[a];
            
            PruebaRendimiento prueba;
            prueba.origen = origen;
            prueba.destino = destino;
            prueba.algoritmo = algo;
            
            // Usar array en stack para evitar problemas de memoria
            const int MAX_CAMINO = 10000;
            int camino[MAX_CAMINO];
            int largo = 0;
            
            auto inicio_tiempo = high_resolution_clock::now();
            double memoria_inicial = obtener_memoria_actual();
            
            try {
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
            } catch (const exception& e) {
                print_safe("Error en algoritmo " + algo + ": " + e.what());
                largo = 0;
            } catch (...) {
                print_safe("Error desconocido en algoritmo " + algo);
                largo = 0;
            }
            
            auto fin_tiempo = high_resolution_clock::now();
            double memoria_final = obtener_memoria_actual();
            
            prueba.tiempo_ms = duration_cast<microseconds>(fin_tiempo - inicio_tiempo).count() / 1000.0;
            prueba.longitud_camino = (largo > MAX_CAMINO) ? 0 : largo; // Validar tamaño
            prueba.memoria_mb = memoria_final - memoria_inicial;
            prueba.encontro_camino = (largo > 0 && largo <= MAX_CAMINO);
            
            resultados_locales.push_back(prueba);
        }
        
        // Mostrar progreso cada 5 pruebas para no saturar la consola
        if ((i - inicio) % 5 == 0) {
            print_safe("Thread " + to_string(thread_id) + ": " + 
                      to_string(i - inicio) + "/" + to_string(fin - inicio) + " completadas");
        }
    }
    
    // Copiar resultados locales al vector principal de forma segura
    {
        lock_guard<mutex> lock(resultados_mutex);
        for (size_t i = 0; i < resultados_locales.size(); ++i) {
            size_t indice_global = inicio * algoritmos.size() + i;
            if (indice_global < resultados.size()) {
                resultados[indice_global] = resultados_locales[i];
            }
        }
    }
    
    print_safe("Thread " + to_string(thread_id) + " completado exitosamente");
}

int main() {
    cout << "=== PROYECTO RUTAS PARTE II: GRAFOS GRANDES (VERSION SEGURA) ===" << endl;
    cout << "Iniciando pruebas de rendimiento..." << endl;
    
    // Configuración más conservadora
    const int NUM_PRUEBAS = 20; // Reducido para estabilidad
    const int NUM_THREADS = min(4, (int)thread::hardware_concurrency()); // Limitar threads
    
    cout << "Threads disponibles: " << thread::hardware_concurrency() << endl;
    cout << "Threads utilizados: " << NUM_THREADS << endl;
    cout << "Numero de pruebas: " << NUM_PRUEBAS << endl;
    
    // Generar grafo grande CON MALLA DE OBSTÁCULOS (CUMPLE REQUISITO PARTE II)
    cout << "\n1. Generando grafo grande CON MALLA DE OBSTACULOS..." << endl;
    cout << "Cumpliendo requisito Parte II: 'Grafo generado a partir de una malla con obstaculos'" << endl;
    auto inicio_construccion = high_resolution_clock::now();
    
    if (!generar_grafo_con_malla_obstaculos()) {
        cerr << "Error al generar el grafo con malla de obstáculos" << endl;
        return 1;
    }
    
    auto fin_construccion = high_resolution_clock::now();
    double tiempo_construccion = duration_cast<milliseconds>(fin_construccion - inicio_construccion).count();
    
    cout << "Grafo generado exitosamente!" << endl;
    cout << "Tiempo de construccion: " << tiempo_construccion << " ms" << endl;
    cout << "Nodos válidos generados: " << obtener_num_nodos_reales() << " (de " << MAX_NODES_LARGE << " objetivo)" << endl;
    cout << "Aristas aproximadas: " << contar_aristas_grandes() << endl;
    cout << "Memoria usada: " << obtener_memoria_actual() << " MB" << endl;
    
    // Generar puntos de prueba
    cout << "\n2. Generando puntos de prueba..." << endl;
    vector<pair<int, int>> puntos_prueba;
    random_device rd;
    mt19937 gen(rd());
    
    int nodos_reales = obtener_num_nodos_reales();
    cout << "Usando " << nodos_reales << " nodos válidos para las pruebas" << endl;
    
    uniform_int_distribution<> dis(0, nodos_reales - 1);
    
    for (int i = 0; i < NUM_PRUEBAS; ++i) {
        int origen = dis(gen);
        int destino = dis(gen);
        while (destino == origen) {
            destino = dis(gen);
        }
        puntos_prueba.push_back({origen, destino});
    }
    
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
        
        threads.emplace_back(ejecutar_pruebas_paralelas_seguras, 
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
    try {
        analizar_resultados(resultados, NUM_PRUEBAS);
        
        // Guardar resultados en archivo
        guardar_resultados_csv(resultados, "resultados_parte2.csv");
        
        cout << "\n=== PRUEBAS COMPLETADAS EXITOSAMENTE ===" << endl;
        cout << "Resultados guardados en: resultados_parte2.csv" << endl;
        cout << "Memoria final: " << obtener_memoria_actual() << " MB" << endl;
        
    } catch (const exception& e) {
        cerr << "Error en análisis: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
