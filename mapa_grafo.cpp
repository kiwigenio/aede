#include <SFML/Graphics.hpp>
#include "grafo_arequipa.h" 

const int ANCHO = 1200;
const int ALTO = 800;

int main() {
    sf::RenderWindow ventana(sf::VideoMode(ANCHO, ALTO), "Mapa de Arequipa - Grafo");

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear(sf::Color::White);

        
        for (int i = 0; i < NODE_COUNT; ++i) {
            for (int j = OFFSET[i]; j < OFFSET[i + 1]; ++j) {
                int destino = NEIGHBOR[j];

                sf::Vertex linea[] = {
                    sf::Vertex(sf::Vector2f(static_cast<float>(POS_X[i]), static_cast<float>(POS_Y[i])), sf::Color(150,150,150)),
                    sf::Vertex(sf::Vector2f(static_cast<float>(POS_X[destino]), static_cast<float>(POS_Y[destino])), sf::Color(150,150,150))
                };
                ventana.draw(linea, 2, sf::Lines);
            }
        }

        // Dibujar nodos
        for (int i = 0; i < NODE_COUNT; ++i) {
            sf::CircleShape nodo(2); 

            
            nodo.setPosition(static_cast<float>(POS_X[i]) - 2.f, static_cast<float>(POS_Y[i]) - 2.f);
            nodo.setFillColor(sf::Color::Red);
            ventana.draw(nodo);
        }

        ventana.display();
    }

    return 0;
}
