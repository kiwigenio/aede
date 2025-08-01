
#include "algoritmos.h"
const double INFINITO = 1e9;

void buscar_DIJKSTRA(int origen, int destino, int camino[], int& largo) {
    double distancia[NODE_COUNT];
    int anterior[NODE_COUNT];
    bool visitado[NODE_COUNT] = { false };

    for (int i = 0; i < NODE_COUNT; ++i) {
        distancia[i] = INFINITO;
        anterior[i] = -1;
    }

    distancia[origen] = 0.0;
    ColaPrioridad cola;
    cola.insertar(origen, 0.0);

    while (!cola.vacia()) {
        int nodoActual = cola.extraer_min();  

        if (visitado[nodoActual]) continue;
        visitado[nodoActual] = true;

        if (nodoActual == destino) break;

        for (int i = OFFSET[nodoActual]; i < OFFSET[nodoActual + 1]; ++i) {
            int vecino = NEIGHBOR[i];
            double peso = WEIGHT[i];
            double nuevaDistancia = distancia[nodoActual] + peso;

            if (nuevaDistancia < distancia[vecino]) {
                distancia[vecino] = nuevaDistancia;
                anterior[vecino] = nodoActual;
                cola.insertar(vecino, nuevaDistancia);
            }
        }
    }

    largo = 0;
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