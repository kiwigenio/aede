#include "grafo_grande.h"
#include "estructuras_grandes.h"
#include <iostream>
#include <cstring>
#include <limits>

using namespace std;

// Dijkstra optimizado para grafos grandes
void buscar_Dijkstra_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    // Usar arrays dinámicos
    float* distancia = new float[MAX_NODES_LARGE];
    bool* visitado = new bool[MAX_NODES_LARGE]();
    int* anterior = new int[MAX_NODES_LARGE];
    
    // Inicializar
    const float INFINITO = numeric_limits<float>::infinity();
    for (int i = 0; i < MAX_NODES_LARGE; ++i) {
        distancia[i] = INFINITO;
        anterior[i] = -1;
    }
    
    distancia[origen] = 0;
    
    ColaPrioridadGrande cola;
    cola.insertar(origen, 0);
    
    bool encontrado = false;
    
    while (!cola.vacia() && !encontrado) {
        int actual = cola.extraer_min();
        
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
            float peso = grafo_global->get_peso(i);
            
            float nueva_distancia = distancia[actual] + peso;
            
            if (nueva_distancia < distancia[vecino]) {
                distancia[vecino] = nueva_distancia;
                anterior[vecino] = actual;
                cola.insertar(vecino, nueva_distancia);
            }
        }
    }
    
    // Reconstruir camino
    largo = 0;
    if (encontrado && anterior[destino] != -1) {
        int actual = destino;
        while (actual != -1) {
            camino[largo++] = actual;
            actual = anterior[actual];
        }
        
        // Invertir camino
        for (int i = 0; i < largo / 2; ++i) {
            int temp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = temp;
        }
    }
    
    delete[] distancia;
    delete[] visitado;
    delete[] anterior;
}

// A* optimizado para grafos grandes
void buscar_AStar_grande(int origen, int destino, int camino[], int& largo) {
    if (!grafo_global) {
        largo = 0;
        return;
    }
    
    // Usar arrays dinámicos
    float* g = new float[MAX_NODES_LARGE];
    float* f = new float[MAX_NODES_LARGE];
    bool* visitado = new bool[MAX_NODES_LARGE]();
    int* anterior = new int[MAX_NODES_LARGE];
    
    // Inicializar
    const float INFINITO = numeric_limits<float>::infinity();
    for (int i = 0; i < MAX_NODES_LARGE; ++i) {
        g[i] = INFINITO;
        f[i] = INFINITO;
        anterior[i] = -1;
    }
    
    g[origen] = 0;
    f[origen] = heuristica_grande(origen, destino);
    
    ColaPrioridadGrande cola;
    cola.insertar(origen, f[origen]);
    
    bool encontrado = false;
    
    while (!cola.vacia() && !encontrado) {
        int actual = cola.extraer_min();
        
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
            float peso = grafo_global->get_peso(i);
            
            float nuevo_g = g[actual] + peso;
            
            if (nuevo_g < g[vecino]) {
                g[vecino] = nuevo_g;
                f[vecino] = nuevo_g + heuristica_grande(vecino, destino);
                anterior[vecino] = actual;
                cola.insertar(vecino, f[vecino]);
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
            int temp = camino[i];
            camino[i] = camino[largo - 1 - i];
            camino[largo - 1 - i] = temp;
        }
    }
    
    delete[] g;
    delete[] f;
    delete[] visitado;
    delete[] anterior;
}
