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
    sf::RenderWindow window(sf::VideoMode(800,600,16), "SFML TEST", sf::Style::Default, settings);
	sandSystem sand;
	sand.populate();

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}


        window.clear();
		//std::cout << "Does this work\n";
		sand.update(Vector2D(0,-1));
		sand.render(window, Vector2D(100,100));

        window.display();
    }

    return EXIT_SUCCESS;
}