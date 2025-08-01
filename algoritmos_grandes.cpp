#include "grafo_grande.h"
#include "estructuras_grandes.h"
#include "metricas.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <windows.h>
#include <psapi.h>

using namespace std;

// BFS optimizado para grafos grandes
void buscar_BFS_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    int num_nodos = grafo_global->get_num_nodos_reales();
    
    // Validar que los nodos están en rango válido
    if (origen < 0 || origen >= num_nodos || destino < 0 || destino >= num_nodos) {
        largo = 0;
        return;
    }
    
    // Usar arrays dinámicos para el número real de nodos
    bool* visitado = new bool[num_nodos]();
    int* anterior = new int[num_nodos];
    
    // Inicializar anterior
    for (int i = 0; i < num_nodos; ++i) {
        anterior[i] = -1;
    }

    ColaGrande cola;
    cola.encolar(origen);
    visitado[origen] = true;

    bool encontrado = false;
    
    // Búsqueda
    while (!cola.vacia() && !encontrado) {
        int actual = cola.desencolar();

        if (actual == destino) {
            encontrado = true;
            break;
        }

        int inicio = grafo_global->get_offset_inicio(actual);
        int fin = grafo_global->get_offset_fin(actual);
        
        for (int i = inicio; i < fin; ++i) {
            int vecino = grafo_global->get_vecino(i);
            if (!visitado[vecino]) {
                cola.encolar(vecino);
                visitado[vecino] = true;
                anterior[vecino] = actual;
            }
        }
    }

    // Reconstruir camino
    largo = 0;
    if (encontrado) {
        int actual = destino;
        while (actual != -1) {
            camino[largo++] = actual;
            actual = anterior[actual];
        }

        // Invertir camino
        for (int i = 0; i < largo / 2; ++i) {
            int tmp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = tmp;
        }
    }
    
    delete[] visitado;
    delete[] anterior;
}

// DFS optimizado para grafos grandes
void buscar_DFS_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    int num_nodos = grafo_global->get_num_nodos_reales();
    
    // Validar que los nodos están en rango válido
    if (origen < 0 || origen >= num_nodos || destino < 0 || destino >= num_nodos) {
        largo = 0;
        return;
    }
    
    bool* visitado = new bool[num_nodos]();
    int* anterior = new int[num_nodos];
    
    for (int i = 0; i < num_nodos; ++i) {
        anterior[i] = -1;
    }

    StackGrande stack;
    stack.apilar(origen);
    
    bool encontrado = false;

    while (!stack.vacio() && !encontrado) {
        int actual = stack.desapilar();
        
        if (visitado[actual]) continue;
        visitado[actual] = true;

        if (actual == destino) {
            encontrado = true;
            break;
        }

        int inicio = grafo_global->get_offset_inicio(actual);
        int fin = grafo_global->get_offset_fin(actual);
        
        for (int i = inicio; i < fin; ++i) {
            int vecino = grafo_global->get_vecino(i);
            if (!visitado[vecino]) {
                stack.apilar(vecino);
                if (anterior[vecino] == -1) {
                    anterior[vecino] = actual;
                }
            }
        }
    }

    // Reconstruir camino
    largo = 0;
    if (encontrado) {
        int actual = destino;
        while (actual != -1) {
            camino[largo++] = actual;
            actual = anterior[actual];
        }

        for (int i = 0; i < largo / 2; ++i) {
            int tmp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = tmp;
        }
    }
    
    delete[] visitado;
    delete[] anterior;
}

// Best First Search optimizado
void buscar_BestFirst_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    int num_nodos = grafo_global->get_num_nodos_reales();
    
    // Validar que los nodos están en rango válido
    if (origen < 0 || origen >= num_nodos || destino < 0 || destino >= num_nodos) {
        largo = 0;
        return;
    }
    
    bool* visitado = new bool[num_nodos]();
    int* anterior = new int[num_nodos];
    
    for (int i = 0; i < num_nodos; ++i) {
        anterior[i] = -1;
    }

    ColaPrioridadGrande pq;
    pq.insertar(origen, heuristica_grande(origen, destino));

    bool encontrado = false;
    
    while (!pq.vacia() && !encontrado) {
        int actual = pq.extraer_min();

        if (actual == destino) {
            encontrado = true;
            break;
        }
        
        if (visitado[actual]) continue;
        visitado[actual] = true;

        int inicio = grafo_global->get_offset_inicio(actual);
        int fin = grafo_global->get_offset_fin(actual);
        
        for (int i = inicio; i < fin; ++i) {
            int vecino = grafo_global->get_vecino(i);
            if (!visitado[vecino]) {
                anterior[vecino] = actual;
                pq.insertar(vecino, heuristica_grande(vecino, destino));
            }
        }
    }
    
    // Reconstruir camino
    largo = 0;
    if (encontrado) {
        int actual = destino;
        while (actual != -1) {
            camino[largo++] = actual;
            actual = anterior[actual];
        }
        
        for (int i = 0; i < largo / 2; ++i) {
            int temp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = temp;
        }
    }
    
    delete[] visitado;
    delete[] anterior;
}

// Dijkstra optimizado para grafos grandes
void buscar_Dijkstra_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    int num_nodos = grafo_global->get_num_nodos_reales();
    
    // Validar que los nodos están en rango válido
    if (origen < 0 || origen >= num_nodos || destino < 0 || destino >= num_nodos) {
        largo = 0;
        return;
    }
    
    bool* visitado = new bool[num_nodos]();
    int* anterior = new int[num_nodos];
    float* distancia = new float[num_nodos];
    
    // Inicializar
    for (int i = 0; i < num_nodos; ++i) {
        anterior[i] = -1;
        distancia[i] = 1e9f;  // Valor muy grande en lugar de INFINITY
    }
    
    distancia[origen] = 0.0f;
    ColaPrioridadGrande pq;
    pq.insertar(origen, 0.0f);

    bool encontrado = false;
    
    while (!pq.vacia() && !encontrado) {
        int actual = pq.extraer_min();
        
        if (actual == destino) {
            encontrado = true;
            break;
        }
        
        if (visitado[actual]) continue;
        visitado[actual] = true;

        int inicio = grafo_global->get_offset_inicio(actual);
        int fin = grafo_global->get_offset_fin(actual);
        
        for (int i = inicio; i < fin; ++i) {
            int vecino = grafo_global->get_vecino(i);
            float peso = grafo_global->get_peso(i);
            
            if (!visitado[vecino]) {
                float nueva_distancia = distancia[actual] + peso;
                if (nueva_distancia < distancia[vecino]) {
                    distancia[vecino] = nueva_distancia;
                    anterior[vecino] = actual;
                    pq.insertar(vecino, nueva_distancia);
                }
            }
        }
    }
    
    // Reconstruir camino
    largo = 0;
    if (encontrado) {
        int actual = destino;
        while (actual != -1) {
            camino[largo++] = actual;
            actual = anterior[actual];
        }
        
        for (int i = 0; i < largo / 2; ++i) {
            int temp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = temp;
        }
    }
    
    delete[] visitado;
    delete[] anterior;
    delete[] distancia;
}

// A* optimizado para grafos grandes
void buscar_AStar_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    int num_nodos = grafo_global->get_num_nodos_reales();
    
    // Validar que los nodos están en rango válido
    if (origen < 0 || origen >= num_nodos || destino < 0 || destino >= num_nodos) {
        largo = 0;
        return;
    }
    
    bool* visitado = new bool[num_nodos]();
    int* anterior = new int[num_nodos];
    float* g_score = new float[num_nodos];
    float* f_score = new float[num_nodos];
    
    // Inicializar
    for (int i = 0; i < num_nodos; ++i) {
        anterior[i] = -1;
        g_score[i] = 1e9f;  // Valor muy grande en lugar de INFINITY
        f_score[i] = 1e9f;
    }
    
    g_score[origen] = 0.0f;
    f_score[origen] = heuristica_grande(origen, destino);
    
    ColaPrioridadGrande pq;
    pq.insertar(origen, f_score[origen]);

    bool encontrado = false;
    
    while (!pq.vacia() && !encontrado) {
        int actual = pq.extraer_min();
        
        if (actual == destino) {
            encontrado = true;
            break;
        }
        
        if (visitado[actual]) continue;
        visitado[actual] = true;

        int inicio = grafo_global->get_offset_inicio(actual);
        int fin = grafo_global->get_offset_fin(actual);
        
        for (int i = inicio; i < fin; ++i) {
            int vecino = grafo_global->get_vecino(i);
            float peso = grafo_global->get_peso(i);
            
            if (!visitado[vecino]) {
                float tentative_g = g_score[actual] + peso;
                
                if (tentative_g < g_score[vecino]) {
                    anterior[vecino] = actual;
                    g_score[vecino] = tentative_g;
                    f_score[vecino] = g_score[vecino] + heuristica_grande(vecino, destino);
                    pq.insertar(vecino, f_score[vecino]);
                }
            }
        }
    }
    
    // Reconstruir camino
    largo = 0;
    if (encontrado) {
        int actual = destino;
        while (actual != -1) {
            camino[largo++] = actual;
            actual = anterior[actual];
        }
        
        for (int i = 0; i < largo / 2; ++i) {
            int temp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = temp;
        }
    }
    
    delete[] visitado;
    delete[] anterior;
    delete[] g_score;
    delete[] f_score;
}

// Función para obtener uso de memoria actual
double obtener_memoria_actual() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024.0 / 1024.0; // MB
    }
    return 0.0;
}

// Función para analizar resultados
void analizar_resultados(const vector<PruebaRendimiento>& resultados, int num_threads) {
    cout << "\n=== ANÁLISIS DE RESULTADOS ===" << endl;
    cout << "Configuración: " << num_threads << " threads" << endl;
    cout << "Total de pruebas: " << resultados.size() << endl;
    
    // Estadísticas por algoritmo
    map<string, vector<double>> tiempos_por_algoritmo;
    map<string, vector<int>> largos_por_algoritmo;
    
    for (const auto& resultado : resultados) {
        if (resultado.encontro_camino) {
            tiempos_por_algoritmo[resultado.algoritmo].push_back(resultado.tiempo_ms);
            largos_por_algoritmo[resultado.algoritmo].push_back(resultado.longitud_camino);
        }
    }
    
    for (const auto& [algoritmo, tiempos] : tiempos_por_algoritmo) {
        if (!tiempos.empty()) {
            double suma = 0;
            for (double t : tiempos) suma += t;
            double promedio = suma / tiempos.size();
            
            cout << "\n" << algoritmo << ":" << endl;
            cout << "  Pruebas exitosas: " << tiempos.size() << endl;
            cout << "  Tiempo promedio: " << fixed << setprecision(2) << promedio << " ms" << endl;
            
            if (!largos_por_algoritmo[algoritmo].empty()) {
                int suma_largos = 0;
                for (int l : largos_por_algoritmo[algoritmo]) suma_largos += l;
                double promedio_largo = (double)suma_largos / largos_por_algoritmo[algoritmo].size();
                cout << "  Largo promedio de camino: " << fixed << setprecision(1) << promedio_largo << endl;
            }
        }
    }
}

// Función para guardar resultados en CSV
void guardar_resultados_csv(const vector<PruebaRendimiento>& resultados, const string& filename) {
    ofstream archivo(filename);
    if (!archivo.is_open()) {
        cerr << "Error al crear archivo CSV: " << filename << endl;
        return;
    }
    
    // Encabezados
    archivo << "Algoritmo,Origen,Destino,TiempoMS,LargoCamino,Exito,MemoriaMB\n";
    
    // Datos
    for (const auto& resultado : resultados) {
        archivo << resultado.algoritmo << ","
                << resultado.origen << ","
                << resultado.destino << ","
                << fixed << setprecision(3) << resultado.tiempo_ms << ","
                << resultado.longitud_camino << ","
                << (resultado.encontro_camino ? "1" : "0") << ","
                << fixed << setprecision(2) << resultado.memoria_mb << "\n";
    }
    
    archivo.close();
    cout << "Resultados guardados en: " << filename << endl;
}
