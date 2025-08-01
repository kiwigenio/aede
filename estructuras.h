
#pragma once

const int TAM_MAX = 10000;
const int PQ_MAX = 10000;

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


struct NodoPrioridad {
    int id;
    float prioridad;
};

class ColaPrioridad {
private:
    NodoPrioridad datos[PQ_MAX];
    int cantidad;

public:
    ColaPrioridad() {
        cantidad = 0;
    }

    bool vacia() const {
        return cantidad == 0;
    }

    void insertar(int id, float prioridad) {
        int i = cantidad++;
        while (i > 0 && prioridad < datos[(i - 1) / 2].prioridad) {
            datos[i] = datos[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        datos[i] = { id, prioridad };
    }

    int extraer_min() {
        if (vacia()) return -1;
        int id = datos[0].id;
        NodoPrioridad ultimo = datos[--cantidad];

        int i = 0, hijo;
        while ((hijo = 2 * i + 1) < cantidad) {
            if (hijo + 1 < cantidad && datos[hijo + 1].prioridad < datos[hijo].prioridad)
                hijo++;
            if (ultimo.prioridad <= datos[hijo].prioridad)
                break;
            datos[i] = datos[hijo];
            i = hijo;
        }
        datos[i] = ultimo;
        return id;
    }
};
