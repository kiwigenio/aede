#include <SFML/Graphics.hpp>
#include "grafo_arequipa.h"

const int ANCHO = 1200;
const int ALTO = 800;

int main() {
    sf::RenderWindow ventana(sf::VideoMode(ANCHO, ALTO), "Mapa de Arequipa - Grafo");
    ventana.setFramerateLimit(60);

    sf::View vista(sf::FloatRect(0, 0, ANCHO, ALTO));
    ventana.setView(vista);

    bool arrastrando = false;
    sf::Vector2i mouseAnterior;

    float zoomFactor = 1.0f;
    const float ZOOM_STEP = 1.1f;

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            // Zoom con rueda del mouse
            if (evento.type == sf::Event::MouseWheelScrolled) {
                if (evento.mouseWheelScroll.delta > 0)
                    zoomFactor /= ZOOM_STEP;
                else
                    zoomFactor *= ZOOM_STEP;

                vista.zoom((evento.mouseWheelScroll.delta > 0) ? 1.0f / ZOOM_STEP : ZOOM_STEP);
                ventana.setView(vista);
            }

            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                arrastrando = true;
                mouseAnterior = sf::Mouse::getPosition(ventana);
            }

            if (evento.type == sf::Event::MouseButtonReleased && evento.mouseButton.button == sf::Mouse::Left) {
                arrastrando = false;
            }

            // Mover vista
            if (evento.type == sf::Event::MouseMoved && arrastrando) {
                sf::Vector2i mouseActual = sf::Mouse::getPosition(ventana);
                sf::Vector2f delta = ventana.mapPixelToCoords(mouseAnterior) - ventana.mapPixelToCoords(mouseActual);
                vista.move(delta);
                ventana.setView(vista);
                mouseAnterior = mouseActual;
            }
        }

        ventana.clear(sf::Color::White);

        for (int i = 0; i < NODE_COUNT; ++i) {
            for (int j = OFFSET[i]; j < OFFSET[i + 1]; ++j) {
                int destino = NEIGHBOR[j];
                sf::Vertex linea[] = {
                    sf::Vertex(sf::Vector2f(static_cast<float>(POS_X[i]), static_cast<float>(POS_Y[i])), sf::Color(200, 200, 200)),
                    sf::Vertex(sf::Vector2f(static_cast<float>(POS_X[destino]), static_cast<float>(POS_Y[destino])), sf::Color(200, 200, 200))
                };
                ventana.draw(linea, 2, sf::Lines);
            }
        }


        float radioNodo = 0.5f;  

        for (int i = 0; i < NODE_COUNT; ++i) {
            sf::CircleShape nodo(radioNodo);
            nodo.setPosition(static_cast<float>(POS_X[i]) - radioNodo, static_cast<float>(POS_Y[i]) - radioNodo);
            nodo.setFillColor(sf::Color::Red);
            ventana.draw(nodo);
        }

        ventana.display();
    }

    return 0;
}
