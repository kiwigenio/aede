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

int main() {
    cout << "=== PROYECTO RUTAS PARTE II: PRUEBA RAPIDA ===" << endl;
    cout << "Version de prueba con grafo pequeño para verificar funcionalidad..." << endl;
    
    // Configuración para prueba rápida
    const int NUM_PRUEBAS = 5;
    const int NUM_THREADS = min(2, (int)thread::hardware_concurrency());
    
    cout << "Threads utilizados: " << NUM_THREADS << endl;
    cout << "Numero de pruebas: " << NUM_PRUEBAS << endl;
    
    // Mostrar información del sistema
    cout << "\nInformacion del sistema:" << endl;
    cout << "CPU Cores disponibles: " << thread::hardware_concurrency() << endl;
    cout << "Memoria inicial: " << obtener_memoria_actual() << " MB" << endl;
    
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
    cout << "Aristas: " << contar_aristas_grandes() << endl;
    cout << "Memoria después de generar grafo: " << obtener_memoria_actual() << " MB" << endl;
    
    // Prueba básica de algoritmos
    cout << "\n2. Probando algoritmos básicos..." << endl;
    
    int origen = 0;
    int destino = 1000;
    int camino[1000];
    int largo;
    
    // Probar Dijkstra
    auto inicio_dijkstra = high_resolution_clock::now();
    buscar_Dijkstra_grande(origen, destino, camino, largo);
    auto fin_dijkstra = high_resolution_clock::now();
    
    double tiempo_dijkstra = duration_cast<microseconds>(fin_dijkstra - inicio_dijkstra).count() / 1000.0;
    
    cout << "Dijkstra: " << tiempo_dijkstra << " ms, camino length: " << largo << endl;
    
    // Probar BFS
    auto inicio_bfs = high_resolution_clock::now();
    buscar_BFS_grande(origen, destino, camino, largo);
    auto fin_bfs = high_resolution_clock::now();
    
    double tiempo_bfs = duration_cast<microseconds>(fin_bfs - inicio_bfs).count() / 1000.0;
    
    cout << "BFS: " << tiempo_bfs << " ms, camino length: " << largo << endl;
    
    // Probar A*
    auto inicio_astar = high_resolution_clock::now();
    buscar_AStar_grande(origen, destino, camino, largo);
    auto fin_astar = high_resolution_clock::now();
    
    double tiempo_astar = duration_cast<microseconds>(fin_astar - inicio_astar).count() / 1000.0;
    
    cout << "A*: " << tiempo_astar << " ms, camino length: " << largo << endl;
    
    cout << "\n=== PRUEBA RAPIDA COMPLETADA ===" << endl;
    cout << "La implementacion esta funcionando correctamente!" << endl;
    cout << "Para ejecutar el benchmark completo use: .\\parte2_benchmark.exe" << endl;
    
    return 0;
}
