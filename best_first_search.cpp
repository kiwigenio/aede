#include "algoritmos.h"

void buscar_BestFirst(int origen, int destino, int camino[], int& largo) {
    bool visitado[NODE_COUNT] = { false };
    int anterior[NODE_COUNT];
    for (int i = 0; i < NODE_COUNT; ++i) anterior[i] = -1;

    ColaPrioridad pq;
    pq.insertar(origen, heuristica(origen, destino));

    while (!pq.vacia()) {
        int actual = pq.extraer_min();

        if (actual == destino) break;
        if (visitado[actual]) continue;

        visitado[actual] = true;

        for (int i = OFFSET[actual]; i < OFFSET[actual + 1]; ++i) {
            int vecino = NEIGHBOR[i];
            if (!visitado[vecino]) {
                anterior[vecino] = actual;
                pq.insertar(vecino, heuristica(vecino, destino));
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
        int temp = camino[i];
        camino[i] = camino[largo - 1 - i];
        camino[largo - 1 - i] = temp;
    }
}