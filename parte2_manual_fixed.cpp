#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <fstream>
#include <memory>
#include <iomanip>
#include <algorithm>
#include "estructuras_grandes.h"
#include "grafo_grande.h"
#include "metricas.h"

using namespace std;
using namespace chrono;

// Función para mostrar el camino detallado de un algoritmo
void mostrar_camino(int origen, int destino, const string& algoritmo) {
    cout << "\n=== CALCULANDO CAMINO DETALLADO ===" << endl;
    cout << "Algoritmo: " << algoritmo << endl;
    cout << "Origen: " << origen << " -> Destino: " << destino << endl;
    
    int camino[10000];
    int largo = 0;
    double tiempo_ms = 0;
    
    auto inicio = high_resolution_clock::now();
    
    if (algoritmo == "BFS") {
        buscar_BFS_grande(origen, destino, camino, largo);
    } else if (algoritmo == "DFS") {
        buscar_DFS_grande(origen, destino, camino, largo);
    } else if (algoritmo == "Dijkstra") {
        buscar_Dijkstra_grande(origen, destino, camino, largo);
    } else if (algoritmo == "AStar") {
        buscar_AStar_grande(origen, destino, camino, largo);
    }
    
    auto fin = high_resolution_clock::now();
    tiempo_ms = duration_cast<microseconds>(fin - inicio).count() / 1000.0;
    
    cout << "\nResultado:" << endl;
    cout << "Tiempo de calculo: " << fixed << setprecision(3) << tiempo_ms << " ms" << endl;
    
    if (largo > 0) {
        cout << "Camino encontrado con " << largo << " nodos" << endl;
        cout << "Camino: ";
        
        // Mostrar solo los primeros y últimos nodos si es muy largo
        if (largo <= 20) {
            for (int i = 0; i < largo; i++) {
                cout << camino[i];
                if (i < largo - 1) cout << " -> ";
            }
        } else {
            // Mostrar los primeros 8 nodos
            for (int i = 0; i < 8; i++) {
                cout << camino[i] << " -> ";
            }
            cout << "... -> ";
            // Mostrar los últimos 8 nodos
            for (int i = largo - 8; i < largo; i++) {
                cout << camino[i];
                if (i < largo - 1) cout << " -> ";
            }
        }
        cout << endl;
        
        // Mostrar estadísticas del camino
        cout << "\nEstadisticas del camino:" << endl;
        cout << "- Longitud total: " << largo << " nodos" << endl;
        cout << "- Distancia: " << largo - 1 << " aristas" << endl;
        
    } else {
        cout << "No se encontro camino entre " << origen << " y " << destino << endl;
    }
}

// Función para validar entrada de nodo
int leer_nodo_valido(const string& mensaje) {
    int nodo;
    while (true) {
        cout << mensaje;
        cin >> nodo;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Por favor ingrese un numero valido." << endl;
            continue;
        }
        
        if (nodo < 0 || nodo >= MAX_NODES_LARGE) {
            cout << "Error: Nodo fuera de rango. Debe estar entre 0 y " 
                 << (MAX_NODES_LARGE - 1) << endl;
            continue;
        }
        
        return nodo;
    }
}

// Función para ejecutar un algoritmo y obtener métricas
PruebaRendimiento ejecutar_algoritmo_con_metricas(const string& algoritmo, int origen, int destino) {
    PruebaRendimiento resultado;
    resultado.algoritmo = algoritmo;
    resultado.origen = origen;
    resultado.destino = destino;
    
    int camino[10000];
    int largo = 0;
    
    double memoria_inicial = obtener_memoria_actual();
    auto inicio = high_resolution_clock::now();
    
    if (algoritmo == "BFS") {
        buscar_BFS_grande(origen, destino, camino, largo);
    } else if (algoritmo == "DFS") {
        buscar_DFS_grande(origen, destino, camino, largo);
    } else if (algoritmo == "BestFirst") {
        buscar_BestFirst_grande(origen, destino, camino, largo);
    } else if (algoritmo == "Dijkstra") {
        buscar_Dijkstra_grande(origen, destino, camino, largo);
    } else if (algoritmo == "AStar") {
        buscar_AStar_grande(origen, destino, camino, largo);
    }
    
    auto fin = high_resolution_clock::now();
    resultado.tiempo_ms = duration_cast<microseconds>(fin - inicio).count() / 1000.0;
    resultado.memoria_mb = obtener_memoria_actual() - memoria_inicial;
    resultado.longitud_camino = largo;
    resultado.encontro_camino = (largo > 0);
    
    return resultado;
}

// Función para generar puntos de prueba aleatorios
vector<pair<int, int>> generar_puntos_prueba_aleatorios(int num_pruebas) {
    vector<pair<int, int>> puntos;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, MAX_NODES_LARGE - 1);
    
    for (int i = 0; i < num_pruebas; i++) {
        int origen, destino;
        do {
            origen = dis(gen);
            destino = dis(gen);
        } while (origen == destino);
        
        puntos.push_back({origen, destino});
    }
    
    return puntos;
}

// Función para benchmark automático
void ejecutar_benchmark_automatico() {
    cout << "\n=== BENCHMARK AUTOMATICO ===" << endl;
    
    // Configuración
    const int NUM_PRUEBAS = 20;
    const vector<string> algoritmos = {"BFS", "DFS", "BestFirst", "Dijkstra", "AStar"};
    
    cout << "Generando " << NUM_PRUEBAS << " pares de nodos aleatorios..." << endl;
    auto puntos_prueba = generar_puntos_prueba_aleatorios(NUM_PRUEBAS);
    
    cout << "Ejecutando benchmark con " << algoritmos.size() << " algoritmos..." << endl;
    
    // Almacenar resultados
    vector<vector<PruebaRendimiento>> resultados(algoritmos.size());
    
    // Ejecutar todas las pruebas
    for (size_t alg_idx = 0; alg_idx < algoritmos.size(); alg_idx++) {
        const string& algoritmo = algoritmos[alg_idx];
        cout << "\nProbando " << algoritmo << "..." << endl;
        
        for (int prueba = 0; prueba < NUM_PRUEBAS; prueba++) {
            int origen = puntos_prueba[prueba].first;
            int destino = puntos_prueba[prueba].second;
            
            auto resultado = ejecutar_algoritmo_con_metricas(algoritmo, origen, destino);
            resultados[alg_idx].push_back(resultado);
            
            cout << "  Prueba " << (prueba + 1) << "/" << NUM_PRUEBAS 
                 << ": " << resultado.tiempo_ms << " ms" << endl;
        }
    }
    
    // Análisis de resultados
    cout << "\n=== ANALISIS DE RESULTADOS ===" << endl;
    cout << left << setw(12) << "Algoritmo" 
         << setw(12) << "T.Promedio"
         << setw(12) << "T.Min"
         << setw(12) << "T.Max"
         << setw(12) << "L.Promedio"
         << setw(8) << "Exitos" << endl;
    cout << string(70, '-') << endl;
    
    for (size_t alg_idx = 0; alg_idx < algoritmos.size(); alg_idx++) {
        const string& algoritmo = algoritmos[alg_idx];
        const auto& res = resultados[alg_idx];
        
        vector<double> tiempos;
        vector<int> longitudes;
        int exitos = 0;
        
        for (const auto& r : res) {
            tiempos.push_back(r.tiempo_ms);
            if (r.encontro_camino) {
                longitudes.push_back(r.longitud_camino);
                exitos++;
            }
        }
        
        if (!tiempos.empty()) {
            double t_promedio = 0;
            for (double t : tiempos) t_promedio += t;
            t_promedio /= tiempos.size();
            
            double t_min = *min_element(tiempos.begin(), tiempos.end());
            double t_max = *max_element(tiempos.begin(), tiempos.end());
            
            double l_promedio = 0;
            if (!longitudes.empty()) {
                for (int l : longitudes) l_promedio += l;
                l_promedio /= longitudes.size();
            }
            
            cout << left << setw(12) << algoritmo
                 << setw(12) << fixed << setprecision(3) << t_promedio
                 << setw(12) << t_min
                 << setw(12) << t_max
                 << setw(12) << setprecision(1) << l_promedio
                 << setw(8) << exitos << "/" << NUM_PRUEBAS << endl;
        }
    }
    
    // Guardar resultados en CSV
    cout << "\nGuardando resultados en resultados_benchmark.csv..." << endl;
    ofstream archivo("resultados_benchmark.csv");
    if (archivo.is_open()) {
        archivo << "Algoritmo,Origen,Destino,Tiempo_ms,Longitud,Memoria_MB,Encontro_Camino\n";
        
        for (size_t alg_idx = 0; alg_idx < algoritmos.size(); alg_idx++) {
            for (const auto& r : resultados[alg_idx]) {
                archivo << r.algoritmo << "," << r.origen << "," << r.destino << ","
                       << r.tiempo_ms << "," << r.longitud_camino << "," 
                       << r.memoria_mb << "," << (r.encontro_camino ? 1 : 0) << "\n";
            }
        }
        archivo.close();
        cout << "Resultados guardados exitosamente!" << endl;
    } else {
        cout << "Error: No se pudo crear el archivo CSV" << endl;
    }
}

// Función para modo manual con nodos ingresados por usuario
void ejecutar_modo_manual() {
    cout << "\n=== MODO MANUAL: INGRESAR NODOS ===" << endl;
    
    while (true) {
        cout << "\n" << string(50, '=') << endl;
        cout << "OPCIONES DE PRUEBA MANUAL:" << endl;
        cout << "1. Probar un algoritmo especifico" << endl;
        cout << "2. Comparar todos los algoritmos" << endl;
        cout << "3. Ver camino detallado" << endl;
        cout << "4. Volver al menu principal" << endl;
        cout << string(50, '=') << endl;
        
        int opcion;
        cout << "Seleccione una opcion (1-4): ";
        cin >> opcion;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Opcion invalida!" << endl;
            continue;
        }
        
        if (opcion == 4) {
            break;
        }
        
        if (opcion >= 1 && opcion <= 3) {
            // Leer nodos del usuario
            cout << "\nIngrese los nodos para la prueba:" << endl;
            int origen = leer_nodo_valido("Nodo origen (0-" + to_string(MAX_NODES_LARGE-1) + "): ");
            int destino = leer_nodo_valido("Nodo destino (0-" + to_string(MAX_NODES_LARGE-1) + "): ");
            
            if (origen == destino) {
                cout << "Error: Origen y destino no pueden ser iguales!" << endl;
                continue;
            }
            
            cout << "\nProbando ruta: " << origen << " -> " << destino << endl;
            
            if (opcion == 1) {
                // Probar un algoritmo específico
                cout << "\nAlgoritmos disponibles:" << endl;
                cout << "1. BFS\n2. DFS\n3. Best First\n4. Dijkstra\n5. A*" << endl;
                
                int algo_opcion;
                cout << "Seleccione algoritmo (1-5): ";
                cin >> algo_opcion;
                
                string algoritmos[] = {"", "BFS", "DFS", "BestFirst", "Dijkstra", "AStar"};
                if (algo_opcion >= 1 && algo_opcion <= 5) {
                    string algoritmo = algoritmos[algo_opcion];
                    PruebaRendimiento resultado = ejecutar_algoritmo_con_metricas(algoritmo, origen, destino);
                    
                    cout << "\n=== RESULTADO ===" << endl;
                    cout << "Algoritmo: " << resultado.algoritmo << endl;
                    cout << "Tiempo: " << fixed << setprecision(3) << resultado.tiempo_ms << " ms" << endl;
                    cout << "Longitud camino: " << resultado.longitud_camino << " nodos" << endl;
                    cout << "Memoria usada: " << resultado.memoria_mb << " MB" << endl;
                    cout << "Exito: " << (resultado.encontro_camino ? "SI" : "NO") << endl;
                } else {
                    cout << "Opcion de algoritmo invalida!" << endl;
                }
                
            } else if (opcion == 2) {
                // Comparar todos los algoritmos
                cout << "\nEjecutando comparacion completa..." << endl;
                
                vector<string> algoritmos = {"BFS", "DFS", "BestFirst", "Dijkstra", "AStar"};
                vector<PruebaRendimiento> resultados;
                
                for (const string& algo : algoritmos) {
                    resultados.push_back(ejecutar_algoritmo_con_metricas(algo, origen, destino));
                }
                
                cout << "\n=== COMPARACION COMPLETA ===" << endl;
                cout << left << setw(12) << "Algoritmo" 
                     << setw(12) << "Tiempo(ms)"
                     << setw(12) << "Longitud"
                     << setw(12) << "Memoria(MB)"
                     << setw(8) << "Exito" << endl;
                cout << string(60, '-') << endl;
                
                for (const auto& r : resultados) {
                    cout << left << setw(12) << r.algoritmo
                         << setw(12) << fixed << setprecision(3) << r.tiempo_ms
                         << setw(12) << r.longitud_camino
                         << setw(12) << r.memoria_mb
                         << setw(8) << (r.encontro_camino ? "SI" : "NO") << endl;
                }
                
                // Análisis
                auto mas_rapido = min_element(resultados.begin(), resultados.end(),
                    [](const PruebaRendimiento& a, const PruebaRendimiento& b) {
                        return a.encontro_camino && (!b.encontro_camino || a.tiempo_ms < b.tiempo_ms);
                    });
                
                auto mejor_camino = min_element(resultados.begin(), resultados.end(),
                    [](const PruebaRendimiento& a, const PruebaRendimiento& b) {
                        return a.encontro_camino && (!b.encontro_camino || a.longitud_camino < b.longitud_camino);
                    });
                
                cout << "\nANALISIS:" << endl;
                if (mas_rapido != resultados.end() && mas_rapido->encontro_camino) {
                    cout << "Mas rapido: " << mas_rapido->algoritmo 
                         << " (" << mas_rapido->tiempo_ms << " ms)" << endl;
                }
                if (mejor_camino != resultados.end() && mejor_camino->encontro_camino) {
                    cout << "Mejor camino: " << mejor_camino->algoritmo 
                         << " (" << mejor_camino->longitud_camino << " nodos)" << endl;
                }
                
            } else if (opcion == 3) {
                // Ver camino detallado
                cout << "\nAlgoritmos para ver camino:" << endl;
                cout << "1. BFS\n2. DFS\n3. Dijkstra\n4. A*" << endl;
                
                int algo_opcion;
                cout << "Seleccione algoritmo (1-4): ";
                cin >> algo_opcion;
                
                string algoritmos[] = {"", "BFS", "DFS", "Dijkstra", "AStar"};
                if (algo_opcion >= 1 && algo_opcion <= 4) {
                    mostrar_camino(origen, destino, algoritmos[algo_opcion]);
                } else {
                    cout << "Opcion de algoritmo invalida!" << endl;
                }
            }
        } else {
            cout << "Opcion invalida!" << endl;
        }
    }
}

int main() {
    cout << "=== PROYECTO RUTAS PARTE II: BENCHMARKS Y PRUEBAS ===" << endl;
    cout << "Sistema completo de pruebas para algoritmos de busqueda" << endl;
    
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
    cout << "Memoria usada: " << obtener_memoria_actual() << " MB" << endl;
    
    // Menu principal
    while (true) {
        cout << "\n" << string(60, '=') << endl;
        cout << "MENU PRINCIPAL - SELECCIONE MODO DE PRUEBA:" << endl;
        cout << "1. MODO AUTOMATICO: Benchmark con nodos aleatorios" << endl;
        cout << "2. MODO MANUAL: Ingresar nodos especificos por teclado" << endl;
        cout << "3. Ver estadisticas del grafo" << endl;
        cout << "4. Salir" << endl;
        cout << string(60, '=') << endl;
        
        int opcion;
        cout << "Seleccione una opcion (1-4): ";
        cin >> opcion;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Opcion invalida. Por favor ingrese un numero del 1 al 4." << endl;
            continue;
        }
        
        switch (opcion) {
            case 1:
                cout << "\n=== MODO AUTOMATICO SELECCIONADO ===" << endl;
                ejecutar_benchmark_automatico();
                break;
                
            case 2:
                cout << "\n=== MODO MANUAL SELECCIONADO ===" << endl;
                ejecutar_modo_manual();
                break;
                
            case 3:
                cout << "\n=== ESTADISTICAS DEL GRAFO ===" << endl;
                cout << "Nodos totales: " << MAX_NODES_LARGE << endl;
                cout << "Aristas totales: " << contar_aristas_grandes() << endl;
                cout << "Memoria utilizada: " << obtener_memoria_actual() << " MB" << endl;
                cout << "Densidad estimada: " << fixed << setprecision(4) 
                     << (double(contar_aristas_grandes()) / (double(MAX_NODES_LARGE) * (MAX_NODES_LARGE - 1))) * 100 
                     << "%" << endl;
                break;
                
            case 4:
                cout << "\nCerrando programa..." << endl;
                return 0;
                
            default:
                cout << "Opcion invalida. Por favor seleccione 1, 2, 3 o 4." << endl;
                break;
        }
    }
    
    return 0;
}
