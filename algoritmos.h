#pragma once
#include "grafo_arequipa.h"
#include "estructuras.h"
#include <cmath>
/*Plantilla de los algoritmos */


/*algoritmo BFS*/
void buscar_BFS(int origen, int destino, int camino[], int& largo);

/*algoritmo DFS*/ 
void buscar_DFS(int origen, int destino, int camino[], int& largo);
void dfs(int actual, int destino, bool visitado[], int anterior[]);

/*algoritmo Best first search*/ 
void buscar_BestFirst(int origen, int destino, int camino[], int& largo);

/*algoritmo A* */ 
void buscar_AStar(int origen, int destino, int camino[], int& largo);

/*algoritmo Dijkstra*/ 
void buscar_DIJKSTRA(int origen, int destino, int camino[], int& largo);

float heuristica(int nodo, int destino);

void intercambiar(int& a, int& b);
