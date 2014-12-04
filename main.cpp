#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Vector2D.h"
#include "GameObject.h"
#include "Sand.h"


int main() {
	std::cout << "We made it here!";
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1000,650,32), "SFML TEST", sf::Style::Default, settings);

    sf::VertexArray shape(sf::Quads, 4);
	
	sf::Texture t;
	t.loadFromFile("TestTexture.jpg");
	sf::Sprite sprite;
	sprite.setTexture(t);

	sandSystem sand;
	sand.populate();

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

        window.clear();

		sand.render(window, Vector2D(0,0));

        window.display();
    }

    return EXIT_SUCCESS;
}