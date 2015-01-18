#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Vector2D.h"
#include "Projectile.h"
#include "Sand.h"
#include "ExplosionType.h"
#include "Tank.h"


// Comments are for those of weak constitution and simple minds
std::vector<Weapon> weapons;

int main() {
	srand(time(NULL));
	time_t start = time(NULL);
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "TANKS", sf::Style::Default, settings);

	sandSystem sand(&window);

	std::vector<Projectile> projectiles;
	std::vector<Tank> tanks;

	sand.populate(150.0, 0.45);
	
	int players = 2;
	int turn = 0;

	Projectile p(Vector2D(100,300),Vector2D(15,1));
	p.alive = true;
	projectiles.push_back(p);

	// Add tanks!
	for (int i = 0; i < players; i++ ) {
		Tank tank;
		tank = Tank(1000); // Tank health is 1K. Adjust damage accordingly.
		Vector2D pos(rand() % SAND_SYSTEM_X,0);
		int y = 0;
		while (sand.staticSand[(int)pos.x][y] != sf::Color::Transparent) y++;
		pos.y = y;
		tank.setPos(pos);

		tanks.push_back(tank);
	}

	sf::Clock timer;

    while (window.isOpen()) {
		sf::Clock deltaTimer;
		tanks[turn].startTurn();
		while (!tanks[turn].controls(deltaTimer.getElapsedTime().asMilliseconds()) && window.isOpen()) {
			deltaTimer.restart();
			timer.restart();
			// Executes until the player shoots.

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			
			timer.restart();
			sand.render();
			std::cout << "Sand render took " << timer.getElapsedTime().asMicroseconds() << " microseconds.\n";

			for (int i = 0; i < tanks.size(); i ++) {
				tanks[i].render(window, i == turn);
			}

			window.display();
			// TODO: Add frame rate lock.
		}
		
		projectiles.push_back(tanks[turn].result);

		turn = (turn+1)%players;

		std::cout << "Switching to other mode! Projectiles: " << projectiles.size() << '\n';

		int updateResult = 1;

		while ((sand.update(Vector2D(0,-1)) || projectiles.size() > 0) && window.isOpen()) {
			//std::cout << "UPDATE took " << timer.getElapsedTime().asMilliseconds() << " milliss.\n";
			
			// This loop will execute until there is no active sand, no projectiles, and no closed window.

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.clear();
			
			timer.restart();
			sand.render();
			//std::cout << "Sand render took " << timer.getElapsedTime().asMilliseconds() << " millis with active particles.\n";

			for (int i = 0; i < tanks.size(); i++) {
				tanks[i].render(window, false);
			}

			for (int i = 0; i < projectiles.size(); i++) {
				//std::cout << "Running projectile " << i << ".\n";
				if (projectiles[i].update(&sand,Vector2D(0,-1))) {
					projectiles[i].render(window);
				} else {
					projectiles.erase(projectiles.begin() + i);
					i--;
				}
			}
			window.display();
			timer.restart();
		}
    }
    return EXIT_SUCCESS;
}

void populateWeapons(std::string filename) {
	// TODO: Get weapons from CSV.
	// TODO: Find a way to count weapons.
}
