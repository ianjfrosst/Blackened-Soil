#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Vector2D.h"
#include "GameObject.h"
#include "Sand.h"

int main() {
	srand(time(NULL));
	time_t start = time(NULL);
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "TANKS", sf::Style::Default, settings);

	sandSystem sand;

	sand.populate(150.0, 0.45);

	bool boom = 0;

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2i position = sf::Mouse::getPosition(window);
			sand.createSand(position.x, SAND_SYSTEM_Y-position.y, sf::Color::Cyan);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i position = sf::Mouse::getPosition(window);
			sand.detonate(Vector2D(position.x,SAND_SYSTEM_Y-position.y), 1, 50);
		}

        window.clear();

		sand.update(Vector2D(0,-0.05));
		sand.render(window, Vector2D(0,0));

        window.display();
    }

    return EXIT_SUCCESS;
}