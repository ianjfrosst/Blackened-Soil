#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Vector2D.h"
#include "GameObject.h"
#include "Sand.h"

int main() {
	time_t start = time(NULL);
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(600, 600, 32), "SFML TEST", sf::Style::Default, settings);

	sandSystem sand;

	sand.populate(75);

	bool boom = 0;

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (difftime(time(NULL), start) > 2 && !boom) {
			sand.detonate(Vector2D(250, 50), 200, 50);
			boom = 1;
		}

        window.clear();

		sand.update(Vector2D(0,-1));
		sand.render(window, Vector2D(50,50));

        window.display();
    }

    return EXIT_SUCCESS;
}