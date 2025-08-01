#include "algoritmos.h"

float heuristica(int nodo, int destino) {
    float dx = POS_X[nodo] - POS_X[destino];
    float dy = POS_Y[nodo] - POS_Y[destino];
    return std::sqrt(dx * dx + dy * dy);
}

void intercambiar(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}


void buscar_AStar(int origen, int destino, int camino[], int& largo) {
    bool visitado[NODE_COUNT] = { false };
    float g[NODE_COUNT], f[NODE_COUNT];
    int anterior[NODE_COUNT];

    for (int i = 0; i < NODE_COUNT; ++i) {
        g[i] = 1e9;
        f[i] = 1e9;
        anterior[i] = -1;
    }

    g[origen] = 0;
    f[origen] = heuristica(origen, destino);

    ColaPrioridad cola;
    cola.insertar(origen, f[origen]);

    while (!cola.vacia()) {
        int actual = cola.extraer_min();
        if (visitado[actual]) continue;
        visitado[actual] = true;

        if (actual == destino)
            break;

        for (int i = OFFSET[actual]; i < OFFSET[actual + 1]; ++i) {
            int vecino = NEIGHBOR[i];
            float peso = WEIGHT[i];
            float nuevo_g = g[actual] + peso;

            if (nuevo_g < g[vecino]) {
                g[vecino] = nuevo_g;
                f[vecino] = nuevo_g + heuristica(vecino, destino);
                anterior[vecino] = actual;
                cola.insertar(vecino, f[vecino]);
            }
        }
    }

    largo = 0;
    int actual = destino;
    while (actual != -1) {
        camino[largo++] = actual;
        actual = anterior[actual];
    }

    for (int i = 0; i < largo / 2; ++i)
        intercambiar(camino[i], camino[largo - 1 - i]);
}
