#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Vector2D.h"
#include "GameObject.h"
#include "Sand.h"
#include "World.h"

int main() {
	srand(time(NULL));
	time_t start = time(NULL);
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "TANKS", sf::Style::Default, settings);
	
	World * world = new World();
	//sandSystem sand;
	//sand.populate(150.0, 0.45);

	world->create();

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Vector2i position = sf::Mouse::getPosition(window);
		if (position.x > 0 && position.x < SAND_SYSTEM_X && position.y > 0 && position.y < SAND_SYSTEM_Y) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

				//sand.createSand(position.x, SAND_SYSTEM_Y-position.y, sf::Color::Cyan);
				world->getMap().createSand(position.x, SAND_SYSTEM_Y-position.y, sf::Color::Cyan);
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//sand.detonate(Vector2D(position.x,SAND_SYSTEM_Y-position.y), 1, 50);
				world->getMap().detonate(Vector2D(position.x,SAND_SYSTEM_Y-position.y), 1, 50);
			}
		}

        window.clear();

		world->update();
		world->render(window);

		//sand.update(Vector2D(0,-0.01));
		//sand.render(window, Vector2D(0,0));

        window.display();
    }
	delete world;

    return EXIT_SUCCESS;
}