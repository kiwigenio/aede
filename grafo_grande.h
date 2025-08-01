#pragma once
#include <vector>
#include <memory>

// Configuración para grafo grande
constexpr int MAX_NODES_LARGE = 2000000;  // 2 millones de nodos
constexpr int AVG_DEGREE = 6;             // Grado promedio por nodo
constexpr int MAX_EDGES_LARGE = MAX_NODES_LARGE * AVG_DEGREE;

// Estructura de grafo optimizada para memoria
class GrafoGrande {
private:
    std::vector<int> offset;               // Offset para cada nodo
    std::vector<int> neighbors;            // Lista de vecinos
    std::vector<float> weights;            // Pesos de las aristas
    std::vector<float> pos_x, pos_y;       // Posiciones de nodos
    
public:
    int num_nodos;                         // Número real de nodos generados
    
public:
    GrafoGrande();
    ~GrafoGrande();
    
    bool inicializar();
    void agregar_arista(int origen, int destino, float peso);
    void agregar_posicion(float x, float y);
    void finalizar_construccion();
    
    // Getters inline para performance
    inline int get_offset_inicio(int nodo) const { return offset[nodo]; }
    inline int get_offset_fin(int nodo) const { return offset[nodo + 1]; }
    inline int get_vecino(int idx) const { return neighbors[idx]; }
    inline float get_peso(int idx) const { return weights[idx]; }
    inline float get_pos_x(int nodo) const { return pos_x[nodo]; }
    inline float get_pos_y(int nodo) const { return pos_y[nodo]; }
    inline int get_num_nodos_reales() const { return num_nodos; }
    
    int contar_aristas() const;
    size_t memoria_usada() const;
};

// Instancia global del grafo
extern std::unique_ptr<GrafoGrande> grafo_global;

// Funciones de inicialización - NUEVAS con malla
bool generar_grafo_grande();                    // Mantener compatibilidad
bool generar_grafo_con_malla_obstaculos();      // NUEVO: Cumple requisito Parte II
bool cargar_grafo_desde_archivo(const std::string& archivo);
int contar_aristas_grandes();
int obtener_num_nodos_reales();                 // NUEVO: Obtener número real de nodos generados

// Algoritmos adaptados para grafo grande
void buscar_BFS_grande(int origen, int destino, int camino[], int& largo);
void buscar_DFS_grande(int origen, int destino, int camino[], int& largo);
void buscar_BestFirst_grande(int origen, int destino, int camino[], int& largo);
void buscar_Dijkstra_grande(int origen, int destino, int camino[], int& largo);
void buscar_AStar_grande(int origen, int destino, int camino[], int& largo);

// Funciones de utilidad
float heuristica_grande(int nodo, int destino);
float distancia_euclidiana(int nodo1, int nodo2);
