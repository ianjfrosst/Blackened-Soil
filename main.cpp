#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Vector2D.h"
#include "Projectile.h"
#include "Sand.h"
#include "ExplosionType.h"

int main() {
	srand(time(NULL));
	time_t start = time(NULL);
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "TANKS", sf::Style::Default, settings);

	sandSystem sand;
	Projectile p(Vector2D(100,300),Vector2D(15,1));

	sand.populate(150.0, 0.45);

	bool boom = 0;
	bool lastLMB = false;

    while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::Vector2i position = sf::Mouse::getPosition(window);
		if (position.x > 0 && position.x < SAND_SYSTEM_X && position.y > 0 && position.y < SAND_SYSTEM_Y) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				sand.createSand(position.x, SAND_SYSTEM_Y-position.y, sf::Color::Cyan);
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lastLMB) {
				sand.detonate(Vector2D(position.x,SAND_SYSTEM_Y-position.y), 40, 50, nullptr, explosionType::calderic);
			}
			lastLMB = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		}

        window.clear();

		sand.update(Vector2D(0,-1));
		p.update(sand,Vector2D(0,-1));


		sand.render(window, Vector2D(0,0));
		p.render(window);

        window.display();
    }

    return EXIT_SUCCESS;
}