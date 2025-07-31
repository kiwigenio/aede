#pragma once
#include "grafo_arequipa.h"
#include "estructuras.h"
/*Plantilla de los algoritmos */


/*algoritmo BFS*/
void buscar_BFS(int origen, int destino, int camino[], int& largo);

/*algoritmo DFS*/ 
void buscar_DFS(int origen, int destino, int camino[], int& largo);