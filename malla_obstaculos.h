#pragma once
#include <vector>
#include <random>

// Configuración de la malla
constexpr int GRID_WIDTH = 1414;   // sqrt(2M) aproximadamente para 2M nodos
constexpr int GRID_HEIGHT = 1414;
constexpr float OBSTACLE_PROBABILITY = 0.15f;  // 15% de obstáculos
constexpr float CELL_SIZE = 1.0f;

// Tipos de celdas
enum class CellType {
    FREE = 0,      // Celda libre (transitable)
    OBSTACLE = 1,  // Obstáculo (no transitable)
    WATER = 2,     // Agua (no transitable)
    BUILDING = 3   // Edificio (no transitable)
};

// Estructura de la malla
class MallaConObstaculos {
private:
    std::vector<std::vector<CellType>> grid;
    std::vector<std::vector<int>> node_ids;  // ID del nodo en cada celda
    int next_node_id;
    
    // Generadores de obstáculos
    void generar_obstaculos_aleatorios(std::mt19937& gen);
    void generar_edificios_rectangulares(std::mt19937& gen);
    void generar_rios_y_lagos(std::mt19937& gen);
    void generar_carreteras_principales();
    
    // Utilidades
    bool es_transitable(int x, int y) const;
    bool en_limites(int x, int y) const;
    void conectar_vecinos_adyacentes();
    float calcular_peso_arista(int x1, int y1, int x2, int y2) const;

public:
    MallaConObstaculos();
    ~MallaConObstaculos() = default;
    
    // Métodos principales
    bool generar_malla();
    void exportar_estadisticas() const;
    
    // Getters
    int get_node_id(int x, int y) const;
    CellType get_cell_type(int x, int y) const;
    int get_total_nodes() const { return next_node_id; }
    
    // Para depuración
    void exportar_malla_imagen(const std::string& filename) const;
    void mostrar_estadisticas_detalladas() const;
};

// Función principal para generar grafo desde malla
bool generar_grafo_desde_malla();

// Funciones de utilidad
void generar_obstaculos_coherentes(std::vector<std::vector<CellType>>& grid, std::mt19937& gen);
void aplicar_suavizado_obstaculos(std::vector<std::vector<CellType>>& grid);
std::pair<float, float> coordenadas_mundo(int grid_x, int grid_y);
