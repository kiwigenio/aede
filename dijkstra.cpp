#include "algoritmos.h"

void buscar_Dijkstra(int origen, int destino, int camino[], int& largo) {
    // Inicializar estructuras
    float distancia[NODE_COUNT];
    bool visitado[NODE_COUNT] = { false };
    int anterior[NODE_COUNT];
    
    // Inicializar distancias a infinito y anteriores a -1
    for (int i = 0; i < NODE_COUNT; ++i) {
        distancia[i] = 1e9;  // Infinito
        anterior[i] = -1;
    }
    
    // Distancia del origen a sí mismo es 0
    distancia[origen] = 0;
    
    // Cola de prioridad para nodos pendientes
    ColaPrioridad cola;
    cola.insertar(origen, 0);
    
    while (!cola.vacia()) {
        int actual = cola.extraer_min();
        
        // Si ya fue visitado, continuar
        if (visitado[actual]) continue;
        
        // Marcar como visitado
        visitado[actual] = true;
        
        // Si llegamos al destino, terminar
        if (actual == destino) break;
        
        // Revisar todos los vecinos del nodo actual
        for (int i = OFFSET[actual]; i < OFFSET[actual + 1]; ++i) {
            int vecino = NEIGHBOR[i];
            float peso = WEIGHT[i];
            
            // Calcular nueva distancia tentativa
            float nueva_distancia = distancia[actual] + peso;
            
            // Si encontramos un camino más corto
            if (nueva_distancia < distancia[vecino]) {
                distancia[vecino] = nueva_distancia;
                anterior[vecino] = actual;
                cola.insertar(vecino, nueva_distancia);
            }
        }
    }
    
    // Reconstruir el camino desde destino hacia origen
    largo = 0;
    int actual = destino;
    
    // Verificar si existe un camino
    if (anterior[destino] == -1 && destino != origen) {
        return; // No hay camino
    }
    
    while (actual != -1) {
        camino[largo++] = actual;
        actual = anterior[actual];
    }
    
    // Invertir el camino para que vaya de origen a destino
    for (int i = 0; i < largo / 2; ++i) {
        int temp = camino[i];
        camino[i] = camino[largo - 1 - i];
        camino[largo - 1 - i] = temp;
    }
}
