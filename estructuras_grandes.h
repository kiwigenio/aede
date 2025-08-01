#pragma once

// Estructuras optimizadas para grafos grandes
const int TAM_MAX_GRANDE = 1000000;
const int PQ_MAX_GRANDE = 1000000;

// Cola optimizada para grafos grandes
class ColaGrande {
private:
    int* datos;
    int frente, fin, capacidad;

public:
    ColaGrande(int cap = TAM_MAX_GRANDE) {
        capacidad = cap;
        datos = new int[capacidad];
        frente = 0;
        fin = 0;
    }
    
    ~ColaGrande() {
        delete[] datos;
    }

    bool vacia() const {
        return frente == fin;
    }

    bool llena() const {
        return ((fin + 1) % capacidad) == frente;
    }

    void encolar(int valor) {
        if (!llena()) {
            datos[fin] = valor;
            fin = (fin + 1) % capacidad;
        }
    }

    int desencolar() {
        if (!vacia()) {
            int val = datos[frente];
            frente = (frente + 1) % capacidad;
            return val;
        }
        return -1;
    }
    
    void limpiar() {
        frente = 0;
        fin = 0;
    }
};

// Nodo para cola de prioridad optimizada
struct NodoPrioridadGrande {
    int id;
    float prioridad;
};

// Cola de prioridad optimizada para grafos grandes
class ColaPrioridadGrande {
private:
    NodoPrioridadGrande* datos;
    int cantidad;
    int capacidad;

public:
    ColaPrioridadGrande(int cap = PQ_MAX_GRANDE) {
        capacidad = cap;
        datos = new NodoPrioridadGrande[capacidad];
        cantidad = 0;
    }
    
    ~ColaPrioridadGrande() {
        delete[] datos;
    }

    bool vacia() const {
        return cantidad == 0;
    }
    
    bool llena() const {
        return cantidad >= capacidad;
    }

    void insertar(int id, float prioridad) {
        if (llena()) return;
        
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
        NodoPrioridadGrande ultimo = datos[--cantidad];

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
    
    void limpiar() {
        cantidad = 0;
    }
};

// Stack optimizado para DFS
class StackGrande {
private:
    int* datos;
    int tope;
    int capacidad;

public:
    StackGrande(int cap = TAM_MAX_GRANDE) {
        capacidad = cap;
        datos = new int[capacidad];
        tope = -1;
    }
    
    ~StackGrande() {
        delete[] datos;
    }

    bool vacio() const {
        return tope == -1;
    }
    
    bool lleno() const {
        return tope >= capacidad - 1;
    }

    void apilar(int valor) {
        if (!lleno()) {
            datos[++tope] = valor;
        }
    }

    int desapilar() {
        if (!vacio()) {
            return datos[tope--];
        }
        return -1;
    }
    
    void limpiar() {
        tope = -1;
    }
};

// Pool de memoria para arrays temporales
template<typename T>
class MemoryPool {
private:
    T* pool;
    bool* usado;
    int capacidad;
    int siguiente_libre;

public:
    MemoryPool(int cap) : capacidad(cap), siguiente_libre(0) {
        pool = new T[capacidad];
        usado = new bool[capacidad]();
    }
    
    ~MemoryPool() {
        delete[] pool;
        delete[] usado;
    }
    
    T* obtener_array(int tamano) {
        if (siguiente_libre + tamano <= capacidad) {
            T* ptr = &pool[siguiente_libre];
            for (int i = 0; i < tamano; ++i) {
                usado[siguiente_libre + i] = true;
            }
            siguiente_libre += tamano;
            return ptr;
        }
        return nullptr; // Pool lleno
    }
    
    void liberar_array(T* ptr, int tamano) {
        int inicio = ptr - pool;
        for (int i = 0; i < tamano; ++i) {
            usado[inicio + i] = false;
        }
        // Recompactar si es necesario
        if (inicio + tamano == siguiente_libre) {
            siguiente_libre = inicio;
        }
    }
    
    void reset() {
        siguiente_libre = 0;
        for (int i = 0; i < capacidad; ++i) {
            usado[i] = false;
        }
    }
};
