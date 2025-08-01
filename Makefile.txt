# Makefile para el proyecto de rutas
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -I"SFML/include"
LIBS = -L"SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system
TARGET = mapa_arequipa

SOURCES = mapa_grafo.cpp bfs.cpp dfs.cpp best_first_search.cpp a_estrella.cpp dijkstra.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Regla principal
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Regla para archivos .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Información sobre controles
help:
	@echo "Controles del programa:"
	@echo "- Click izquierdo: Seleccionar nodo origen/destino"
	@echo "- Click derecho + arrastrar: Mover vista"
	@echo "- Scroll: Zoom"
	@echo "- B: BFS"
	@echo "- D: DFS" 
	@echo "- G: Best First Search"
	@echo "- J: Dijkstra"
	@echo "- A: A*"
	@echo "- R: Reset selección"

.PHONY: clean run help
