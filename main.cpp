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
    sf::RenderWindow window(sf::VideoMode(500, 500, 32), "TANKS", sf::Style::Default, settings);

	sandSystem sand;

	sand.populate(150.0, 0.4);

	bool boom = 0;

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (difftime(time(NULL), start) > 1 && !boom) {
			sand.detonate(Vector2D(250, 140), 100, 20);
			boom = 1;
		}

        window.clear();

		sand.update(Vector2D(0,-0.098));
		sand.render(window, Vector2D(0,0));

        window.display();
    }

    return EXIT_SUCCESS;
}