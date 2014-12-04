#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Vector2D.h"
#include "GameObject.h"
#include "Sand.h"


int main() {
    sf::ContextSettings settings;

    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000,650,32), "SFML TEST", sf::Style::Default, settings);

    sf::VertexArray shape(sf::Quads, 4);

	sf::Texture t;
	t.loadFromFile("TestTexture.jpg");
	sf::Sprite sprite;
	sprite.setTexture(t);

    shape[0].position = sf::Vector2f(0, 0);
    shape[1].position = sf::Vector2f(0, 300);
    shape[2].position = sf::Vector2f(300, 300);
    shape[3].position = sf::Vector2f(300, 0);
    
    shape[0].color = sf::Color::Black;
    shape[1].color = sf::Color::Red;
    shape[2].color = sf::Color::Black;
    shape[3].color = sf::Color::Red;

	sandSystem sand();

	sand.populate();
	sand.render();

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

        window.clear();

        window.draw(shape);
		window.draw(sprite);

        window.display();
    }

    return EXIT_SUCCESS;
}