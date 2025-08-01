#include "metricas.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#include <unistd.h>
#endif

using namespace std;

double obtener_memoria_actual() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024.0 / 1024.0; // MB
    }
    return 0.0;
#else
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss / 1024.0; // MB (en Linux ru_maxrss está en KB)
#endif
}

void mostrar_uso_memoria() {
    double memoria = obtener_memoria_actual();
    cout << "Memoria actual: " << fixed << setprecision(2) << memoria << " MB" << endl;
}

ComparacionRendimiento calcular_estadisticas(const vector<PruebaRendimiento>& resultados) {
    ComparacionRendimiento comp;
    
    // Agrupar por algoritmo
    map<string, vector<PruebaRendimiento>> por_algoritmo;
    
    for (const auto& prueba : resultados) {
        por_algoritmo[prueba.algoritmo].push_back(prueba);
    }
    
    // Calcular estadísticas para cada algoritmo
    for (const auto& [nombre, pruebas] : por_algoritmo) {
        EstadisticasAlgoritmo stats;
        stats.nombre = nombre;
        stats.caminos_totales = pruebas.size();
        stats.caminos_encontrados = 0;
        
        vector<double> tiempos, memorias, longitudes;
        
        for (const auto& prueba : pruebas) {
            tiempos.push_back(prueba.tiempo_ms);
            memorias.push_back(prueba.memoria_mb);
            
            if (prueba.encontro_camino) {
                stats.caminos_encontrados++;
                longitudes.push_back(prueba.longitud_camino);
            }
        }
        
        // Estadísticas de tiempo
        stats.tiempo_promedio_ms = accumulate(tiempos.begin(), tiempos.end(), 0.0) / tiempos.size();
        stats.tiempo_min_ms = *min_element(tiempos.begin(), tiempos.end());
        stats.tiempo_max_ms = *max_element(tiempos.begin(), tiempos.end());
        
        // Desviación estándar del tiempo
        double suma_cuadrados = 0;
        for (double tiempo : tiempos) {
            suma_cuadrados += (tiempo - stats.tiempo_promedio_ms) * (tiempo - stats.tiempo_promedio_ms);
        }
        stats.tiempo_desv_std = sqrt(suma_cuadrados / tiempos.size());
        
        // Estadísticas de memoria
        stats.memoria_promedio_mb = accumulate(memorias.begin(), memorias.end(), 0.0) / memorias.size();
        stats.memoria_max_mb = *max_element(memorias.begin(), memorias.end());
        
        // Estadísticas de calidad
        stats.tasa_exito = (double)stats.caminos_encontrados / stats.caminos_totales * 100.0;
        
        if (!longitudes.empty()) {
            stats.longitud_promedio = accumulate(longitudes.begin(), longitudes.end(), 0.0) / longitudes.size();
            stats.longitud_min = *min_element(longitudes.begin(), longitudes.end());
            stats.longitud_max = *max_element(longitudes.begin(), longitudes.end());
        } else {
            stats.longitud_promedio = 0;
            stats.longitud_min = 0;
            stats.longitud_max = 0;
        }
        
        comp.stats[nombre] = stats;
    }
    
    // Determinar mejores algoritmos
    double mejor_tiempo = numeric_limits<double>::max();
    double peor_tiempo = 0;
    double mejor_memoria = numeric_limits<double>::max();
    double mejor_calidad = 0;
    
    for (const auto& [nombre, stats] : comp.stats) {
        if (stats.tiempo_promedio_ms < mejor_tiempo) {
            mejor_tiempo = stats.tiempo_promedio_ms;
            comp.algoritmo_mas_rapido = nombre;
        }
        if (stats.tiempo_promedio_ms > peor_tiempo) {
            peor_tiempo = stats.tiempo_promedio_ms;
            comp.algoritmo_mas_lento = nombre;
        }
        if (stats.memoria_promedio_mb < mejor_memoria) {
            mejor_memoria = stats.memoria_promedio_mb;
            comp.algoritmo_mejor_memoria = nombre;
        }
        if (stats.longitud_promedio > 0 && (mejor_calidad == 0 || stats.longitud_promedio < mejor_calidad)) {
            mejor_calidad = stats.longitud_promedio;
            comp.algoritmo_mejor_calidad = nombre;
        }
    }
    
    return comp;
}

void mostrar_resumen_ejecutivo(const ComparacionRendimiento& comp) {
    cout << "\n=== RESUMEN EJECUTIVO ===" << endl;
    cout << "Algoritmo mas rapido: " << comp.algoritmo_mas_rapido << endl;
    cout << "Algoritmo mas lento: " << comp.algoritmo_mas_lento << endl;
    cout << "Mejor uso de memoria: " << comp.algoritmo_mejor_memoria << endl;
    cout << "Mejor calidad de ruta: " << comp.algoritmo_mejor_calidad << endl;
}

void mostrar_estadisticas_detalladas(const ComparacionRendimiento& comp) {
    cout << "\n=== ESTADISTICAS DETALLADAS ===" << endl;
    cout << fixed << setprecision(3);
    
    cout << left << setw(12) << "Algoritmo"
         << setw(12) << "T.Prom(ms)"
         << setw(12) << "T.Min(ms)" 
         << setw(12) << "T.Max(ms)"
         << setw(12) << "Mem(MB)"
         << setw(12) << "Exito(%)"
         << setw(12) << "L.Prom" << endl;
    
    cout << string(84, '-') << endl;
    
    for (const auto& [nombre, stats] : comp.stats) {
        cout << left << setw(12) << nombre
             << setw(12) << stats.tiempo_promedio_ms
             << setw(12) << stats.tiempo_min_ms
             << setw(12) << stats.tiempo_max_ms
             << setw(12) << stats.memoria_promedio_mb
             << setw(12) << stats.tasa_exito
             << setw(12) << stats.longitud_promedio << endl;
    }
}

void analizar_resultados(const vector<PruebaRendimiento>& resultados, int num_pruebas) {
    cout << "\n=== ANALISIS DE RESULTADOS ===" << endl;
    cout << "Total de pruebas realizadas: " << resultados.size() << endl;
    cout << "Pruebas por algoritmo: " << num_pruebas << endl;
    
    ComparacionRendimiento comp = calcular_estadisticas(resultados);
    
    mostrar_resumen_ejecutivo(comp);
    mostrar_estadisticas_detalladas(comp);
    
    // Análisis adicional
    cout << "\n=== ANALISIS COMPARATIVO ===" << endl;
    
    if (comp.stats.count("Dijkstra") && comp.stats.count("AStar")) {
        double speedup = comp.stats["Dijkstra"].tiempo_promedio_ms / comp.stats["AStar"].tiempo_promedio_ms;
        cout << "A* es " << speedup << "x " << (speedup > 1 ? "mas rapido" : "mas lento") << " que Dijkstra" << endl;
    }
    
    if (comp.stats.count("BFS") && comp.stats.count("Dijkstra")) {
        double speedup = comp.stats["Dijkstra"].tiempo_promedio_ms / comp.stats["BFS"].tiempo_promedio_ms;
        cout << "BFS es " << speedup << "x " << (speedup < 1 ? "mas rapido" : "mas lento") << " que Dijkstra" << endl;
    }
}

void guardar_resultados_csv(const vector<PruebaRendimiento>& resultados, const string& archivo) {
    ofstream file(archivo);
    
    if (!file.is_open()) {
        cerr << "Error al abrir archivo CSV: " << archivo << endl;
        return;
    }
    
    // Header
    file << "Origen,Destino,Algoritmo,Tiempo_ms,Longitud_Camino,Memoria_MB,Encontro_Camino\n";
    
    // Datos
    for (const auto& resultado : resultados) {
        file << resultado.origen << ","
             << resultado.destino << ","
             << resultado.algoritmo << ","
             << resultado.tiempo_ms << ","
             << resultado.longitud_camino << ","
             << resultado.memoria_mb << ","
             << (resultado.encontro_camino ? "1" : "0") << "\n";
    }
    
    file.close();
    cout << "Resultados guardados en: " << archivo << endl;
}

void generar_reporte_html(const ComparacionRendimiento& comp, const string& archivo) {
    ofstream file(archivo);
    
    if (!file.is_open()) {
        cerr << "Error al crear reporte HTML: " << archivo << endl;
        return;
    }
    
    file << "<!DOCTYPE html>\n<html>\n<head>\n";
    file << "<title>Reporte de Rendimiento - Algoritmos de Rutas</title>\n";
    file << "<style>\n";
    file << "body { font-family: Arial, sans-serif; margin: 40px; }\n";
    file << "table { border-collapse: collapse; width: 100%; margin: 20px 0; }\n";
    file << "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
    file << "th { background-color: #f2f2f2; }\n";
    file << ".highlight { background-color: #e8f5e8; }\n";
    file << "</style>\n</head>\n<body>\n";
    
    file << "<h1>Reporte de Rendimiento - Algoritmos de Rutas</h1>\n";
    file << "<h2>Resumen Ejecutivo</h2>\n";
    file << "<ul>\n";
    file << "<li><strong>Algoritmo mas rapido:</strong> " << comp.algoritmo_mas_rapido << "</li>\n";
    file << "<li><strong>Mejor uso de memoria:</strong> " << comp.algoritmo_mejor_memoria << "</li>\n";
    file << "<li><strong>Mejor calidad:</strong> " << comp.algoritmo_mejor_calidad << "</li>\n";
    file << "</ul>\n";
    
    file << "<h2>Estadisticas Detalladas</h2>\n";
    file << "<table>\n<tr>\n";
    file << "<th>Algoritmo</th><th>Tiempo Promedio (ms)</th><th>Memoria (MB)</th>";
    file << "<th>Tasa de Exito (%)</th><th>Longitud Promedio</th>\n</tr>\n";
    
    for (const auto& [nombre, stats] : comp.stats) {
        string clase = (nombre == comp.algoritmo_mas_rapido) ? " class=\"highlight\"" : "";
        file << "<tr" << clase << ">\n";
        file << "<td>" << nombre << "</td>\n";
        file << "<td>" << fixed << setprecision(3) << stats.tiempo_promedio_ms << "</td>\n";
        file << "<td>" << stats.memoria_promedio_mb << "</td>\n";
        file << "<td>" << stats.tasa_exito << "</td>\n";
        file << "<td>" << stats.longitud_promedio << "</td>\n";
        file << "</tr>\n";
    }
    
    file << "</table>\n</body>\n</html>";
    file.close();
    
    cout << "Reporte HTML generado: " << archivo << endl;
}
