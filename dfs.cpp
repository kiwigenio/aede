#include "algoritmos.h"

bool encontrado = false;

void dfs(int actual, int destino, bool visitado[], int anterior[]) {
    if (encontrado) return;

    visitado[actual] = true;

    if (actual == destino) {
        encontrado = true;
        return;
    }

    for (int i = OFFSET[actual]; i < OFFSET[actual + 1]; ++i) {
        int vecino = NEIGHBOR[i];
        if (!visitado[vecino]) {
            anterior[vecino] = actual;
            dfs(vecino, destino, visitado, anterior);
        }
    }
}

void buscar_DFS(int origen, int destino, int camino[], int& largo) {
 
    bool* visitado = new bool[NODE_COUNT]();
    int* anterior = new int[NODE_COUNT];
    for (int i = 0; i < NODE_COUNT; ++i)
        anterior[i] = -1;

    encontrado = false;
    dfs(origen, destino, visitado, anterior);
    largo = 0;
    if (!encontrado) {
        delete[] visitado;
        delete[] anterior;
        return;
    }

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

    delete[] visitado;
    delete[] anterior;
}
