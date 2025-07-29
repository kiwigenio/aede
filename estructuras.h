
#pragma once

const int TAM_MAX = 10000;

class ColaInt {
private:
    int datos[TAM_MAX];
    int frente, fin;

public:
    ColaInt() {
        frente = 0;
        fin = 0;
    }

    bool vacia() const {
        return frente == fin;
    }

    bool llena() const {
        return ((fin + 1) % TAM_MAX) == frente;
    }

    void encolar(int valor) {
        if (!llena()) {
            datos[fin] = valor;
            fin = (fin + 1) % TAM_MAX;
        }
    }

    int desencolar() {
        if (!vacia()) {
            int val = datos[frente];
            frente = (frente + 1) % TAM_MAX;
            return val;
        }
        return -1; 
    }
};
