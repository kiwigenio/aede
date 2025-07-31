#pragma once
#include "grafo_arequipa.h"
#include "estructuras.h"
#include <cmath>
/*Plantilla de los algoritmos */


/*algoritmo BFS*/
void buscar_BFS(int origen, int destino, int camino[], int& largo);

/*algoritmo DFS*/ 
void buscar_DFS(int origen, int destino, int camino[], int& largo);

/*algoritmo Best first search*/ 
void buscar_BestFirst(int origen, int destino, int camino[], int& largo);