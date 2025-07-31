#pragma once

#include "algoritmos.h"

void buscar_BFS(int origen, int destino, int camino[], int& largo) {

    bool visitado[NODE_COUNT] = { false };
    int anterior[NODE_COUNT];

    for (int i = 0; i < NODE_COUNT; ++i)
        anterior[i] = -1;

    ColaInt cola;
    cola.encolar(origen);
    visitado[origen] = true;

    // busqueda
    while (!cola.vacia()) {
        int actual = cola.desencolar();

        if (actual == destino)
            break;

        for (int i = OFFSET[actual]; i < OFFSET[actual + 1]; ++i) {
            int vecino = NEIGHBOR[i];
            if (!visitado[vecino]) {
                cola.encolar(vecino);
                visitado[vecino] = true;
                anterior[vecino] = actual;
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
