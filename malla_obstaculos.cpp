#include "malla_obstaculos.h"
#include "grafo_grande.h"
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

MallaConObstaculos::MallaConObstaculos() : next_node_id(0) {
    grid.resize(GRID_HEIGHT, vector<CellType>(GRID_WIDTH, CellType::FREE));
    node_ids.resize(GRID_HEIGHT, vector<int>(GRID_WIDTH, -1));
}

bool MallaConObstaculos::generar_malla() {
    cout << "Generando malla de " << GRID_WIDTH << "x" << GRID_HEIGHT << " con obstaculos..." << endl;
    
    random_device rd;
    mt19937 gen(rd());
    
    // 1. Generar diferentes tipos de obstáculos
    cout << "1. Generando obstaculos aleatorios..." << endl;
    generar_obstaculos_aleatorios(gen);
    
    cout << "2. Generando edificios y bloques..." << endl;
    generar_edificios_rectangulares(gen);
    
    cout << "3. Generando rios y lagos..." << endl;
    generar_rios_y_lagos(gen);
    
    cout << "4. Creando carreteras principales..." << endl;
    generar_carreteras_principales();
    
    cout << "5. Aplicando suavizado..." << endl;
    aplicar_suavizado_obstaculos(grid);
    
    // 2. Asignar IDs a nodos transitables
    cout << "6. Asignando IDs a nodos transitables..." << endl;
    next_node_id = 0;
    
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (es_transitable(x, y)) {
                node_ids[y][x] = next_node_id++;
            }
        }
        
        if (y % 100 == 0) {
            cout << "Procesando fila " << y << "/" << GRID_HEIGHT << endl;
        }
    }
    
    cout << "Nodos transitables generados: " << next_node_id << endl;
    
    // 3. Verificar que tenemos aproximadamente 2M nodos
    if (next_node_id < 1800000) {
        cout << "ADVERTENCIA: Solo se generaron " << next_node_id << " nodos. Reduciendo obstaculos..." << endl;
        // Reducir obstáculos si no hay suficientes nodos
        for (int y = 0; y < GRID_HEIGHT; y += 3) {
            for (int x = 0; x < GRID_WIDTH; x += 3) {
                if (grid[y][x] == CellType::OBSTACLE) {
                    grid[y][x] = CellType::FREE;
                }
            }
        }
        
        // Reasignar IDs
        next_node_id = 0;
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            for (int x = 0; x < GRID_WIDTH; ++x) {
                if (es_transitable(x, y)) {
                    node_ids[y][x] = next_node_id++;
                }
            }
        }
        cout << "Nodos después de optimización: " << next_node_id << endl;
    }
    
    return true;
}

void MallaConObstaculos::generar_obstaculos_aleatorios(mt19937& gen) {
    uniform_real_distribution<> prob_dist(0.0, 1.0);
    uniform_int_distribution<> type_dist(1, 3);  // OBSTACLE, WATER, BUILDING
    
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (prob_dist(gen) < OBSTACLE_PROBABILITY) {
                grid[y][x] = static_cast<CellType>(type_dist(gen));
            }
        }
    }
}

void MallaConObstaculos::generar_edificios_rectangulares(mt19937& gen) {
    uniform_int_distribution<> x_dist(0, GRID_WIDTH - 1);
    uniform_int_distribution<> y_dist(0, GRID_HEIGHT - 1);
    uniform_int_distribution<> size_dist(5, 25);  // Tamaño de edificios
    
    int num_edificios = 500;  // Número de edificios grandes
    
    for (int i = 0; i < num_edificios; ++i) {
        int start_x = x_dist(gen);
        int start_y = y_dist(gen);
        int width = size_dist(gen);
        int height = size_dist(gen);
        
        for (int y = start_y; y < min(start_y + height, GRID_HEIGHT); ++y) {
            for (int x = start_x; x < min(start_x + width, GRID_WIDTH); ++x) {
                grid[y][x] = CellType::BUILDING;
            }
        }
    }
}

void MallaConObstaculos::generar_rios_y_lagos(mt19937& gen) {
    // Generar ríos serpenteantes
    uniform_int_distribution<> start_dist(0, GRID_WIDTH - 1);
    uniform_int_distribution<> dir_dist(-1, 1);
    
    int num_rios = 20;
    
    for (int r = 0; r < num_rios; ++r) {
        int x = start_dist(gen);
        int y = 0;
        // int direction = 1;  // Dirección general hacia abajo (no usado actualmente)
        
        while (y < GRID_HEIGHT) {
            // Crear río de ancho variable
            int width = 2 + (gen() % 4);  // Ancho 2-5
            
            for (int w = 0; w < width; ++w) {
                if (en_limites(x + w, y)) {
                    grid[y][x + w] = CellType::WATER;
                }
            }
            
            // Avanzar con serpenteo
            y += 1;
            x += dir_dist(gen);  // Serpentear
            
            // Mantener dentro de límites
            x = max(0, min(x, GRID_WIDTH - 5));
        }
    }
    
    // Generar lagos circulares
    int num_lagos = 30;
    uniform_int_distribution<> lago_x_dist(20, GRID_WIDTH - 20);
    uniform_int_distribution<> lago_y_dist(20, GRID_HEIGHT - 20);
    uniform_int_distribution<> radio_dist(5, 15);
    
    for (int l = 0; l < num_lagos; ++l) {
        int centro_x = lago_x_dist(gen);
        int centro_y = lago_y_dist(gen);
        int radio = radio_dist(gen);
        
        for (int y = centro_y - radio; y <= centro_y + radio; ++y) {
            for (int x = centro_x - radio; x <= centro_x + radio; ++x) {
                if (en_limites(x, y)) {
                    float dist = sqrt((x - centro_x) * (x - centro_x) + (y - centro_y) * (y - centro_y));
                    if (dist <= radio) {
                        grid[y][x] = CellType::WATER;
                    }
                }
            }
        }
    }
}

void MallaConObstaculos::generar_carreteras_principales() {
    // Carreteras horizontales principales
    for (int i = 0; i < 10; ++i) {
        int y = (i + 1) * GRID_HEIGHT / 11;  // Distribuir uniformemente
        for (int x = 0; x < GRID_WIDTH; ++x) {
            // Carretera de ancho 3
            for (int offset = -1; offset <= 1; ++offset) {
                if (en_limites(x, y + offset)) {
                    grid[y + offset][x] = CellType::FREE;
                }
            }
        }
    }
    
    // Carreteras verticales principales
    for (int i = 0; i < 10; ++i) {
        int x = (i + 1) * GRID_WIDTH / 11;
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            // Carretera de ancho 3
            for (int offset = -1; offset <= 1; ++offset) {
                if (en_limites(x + offset, y)) {
                    grid[y][x + offset] = CellType::FREE;
                }
            }
        }
    }
}

bool MallaConObstaculos::es_transitable(int x, int y) const {
    if (!en_limites(x, y)) return false;
    return grid[y][x] == CellType::FREE;
}

bool MallaConObstaculos::en_limites(int x, int y) const {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

int MallaConObstaculos::get_node_id(int x, int y) const {
    if (!en_limites(x, y)) return -1;
    return node_ids[y][x];
}

CellType MallaConObstaculos::get_cell_type(int x, int y) const {
    if (!en_limites(x, y)) return CellType::OBSTACLE;
    return grid[y][x];
}

void MallaConObstaculos::exportar_estadisticas() const {
    int total_celdas = GRID_WIDTH * GRID_HEIGHT;
    int libres = 0, obstaculos = 0, agua = 0, edificios = 0;
    
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            switch (grid[y][x]) {
                case CellType::FREE: libres++; break;
                case CellType::OBSTACLE: obstaculos++; break;
                case CellType::WATER: agua++; break;
                case CellType::BUILDING: edificios++; break;
            }
        }
    }
    
    cout << "\n=== ESTADISTICAS DE LA MALLA ===" << endl;
    cout << "Dimensiones: " << GRID_WIDTH << "x" << GRID_HEIGHT << endl;
    cout << "Total de celdas: " << total_celdas << endl;
    cout << "Celdas libres: " << libres << " (" << (100.0 * libres / total_celdas) << "%)" << endl;
    cout << "Obstaculos: " << obstaculos << " (" << (100.0 * obstaculos / total_celdas) << "%)" << endl;
    cout << "Agua: " << agua << " (" << (100.0 * agua / total_celdas) << "%)" << endl;
    cout << "Edificios: " << edificios << " (" << (100.0 * edificios / total_celdas) << "%)" << endl;
    cout << "Nodos transitables: " << next_node_id << endl;
}

void aplicar_suavizado_obstaculos(vector<vector<CellType>>& grid) {
    // Aplicar filtro de suavizado para hacer obstáculos más coherentes
    vector<vector<CellType>> temp_grid = grid;
    
    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
        for (int x = 1; x < GRID_WIDTH - 1; ++x) {
            // Contar vecinos del mismo tipo
            map<CellType, int> vecinos_count;
            
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    vecinos_count[grid[y + dy][x + dx]]++;
                }
            }
            
            // Si mayoría de vecinos son de un tipo, cambiar al tipo mayoritario
            CellType tipo_mayoritario = CellType::FREE;
            int max_count = 0;
            
            for (const auto& [tipo, count] : vecinos_count) {
                if (count > max_count) {
                    max_count = count;
                    tipo_mayoritario = tipo;
                }
            }
            
            if (max_count >= 6) {  // Si 6 o más vecinos son del mismo tipo
                temp_grid[y][x] = tipo_mayoritario;
            }
        }
    }
    
    grid = temp_grid;
}

pair<float, float> coordenadas_mundo(int grid_x, int grid_y) {
    return {grid_x * CELL_SIZE, grid_y * CELL_SIZE};
}

// Función para integrar con el sistema existente
bool generar_grafo_desde_malla() {
    static MallaConObstaculos malla;
    
    cout << "=== GENERANDO GRAFO DESDE MALLA CON OBSTACULOS ===" << endl;
    
    if (!malla.generar_malla()) {
        cerr << "Error al generar la malla" << endl;
        return false;
    }
    
    malla.exportar_estadisticas();
    
    // Verificar que tenemos suficientes nodos
    int total_nodos = malla.get_total_nodes();
    if (total_nodos < 1000000) {
        cout << "ADVERTENCIA: Solo " << total_nodos << " nodos generados (objetivo: ~2M)" << endl;
    }
    
    cout << "Malla con obstaculos generada exitosamente!" << endl;
    cout << "Nodos transitables: " << total_nodos << endl;
    
    return true;
}
