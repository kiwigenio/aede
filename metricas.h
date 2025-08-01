#pragma once
#include <vector>
#include <string>
#include <map>

// Estructura para pruebas de rendimiento
struct PruebaRendimiento {
    int origen;
    int destino;
    std::string algoritmo;
    double tiempo_ms;
    int longitud_camino;
    double memoria_mb;
    bool encontro_camino;
};

// Estructura para estadísticas por algoritmo
struct EstadisticasAlgoritmo {
    std::string nombre;
    double tiempo_promedio_ms;
    double tiempo_min_ms;
    double tiempo_max_ms;
    double tiempo_desv_std;
    
    double memoria_promedio_mb;
    double memoria_max_mb;
    
    int caminos_encontrados;
    int caminos_totales;
    double tasa_exito;
    
    double longitud_promedio;
    double longitud_min;
    double longitud_max;
};

// Estructura para comparación de algoritmos
struct ComparacionRendimiento {
    std::map<std::string, EstadisticasAlgoritmo> stats;
    std::string algoritmo_mas_rapido;
    std::string algoritmo_mas_lento;
    std::string algoritmo_mejor_memoria;
    std::string algoritmo_mejor_calidad;
};

// Funciones de análisis
void analizar_resultados(const std::vector<PruebaRendimiento>& resultados, int num_pruebas);
ComparacionRendimiento calcular_estadisticas(const std::vector<PruebaRendimiento>& resultados);
void mostrar_resumen_ejecutivo(const ComparacionRendimiento& comp);
void mostrar_estadisticas_detalladas(const ComparacionRendimiento& comp);

// Funciones de exportación
void guardar_resultados_csv(const std::vector<PruebaRendimiento>& resultados, 
                           const std::string& archivo);
void generar_reporte_html(const ComparacionRendimiento& comp, 
                         const std::string& archivo);

// Funciones de memoria
double obtener_memoria_actual();
void mostrar_uso_memoria();

// Funciones de benchmark
void benchmark_construccion_grafo();
void benchmark_algoritmos_secuencial();
void benchmark_algoritmos_paralelo(int num_threads);
