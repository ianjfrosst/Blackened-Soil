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

int playGame(sf::RenderWindow&);

int main() {
	sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "Blackened Soil", sf::Style::Default, settings);
	playGame(window);

	
}

int playGame(sf::RenderWindow & window) {
	srand(time(NULL));
	time_t start = time(NULL);
	

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

	int lastMouseState = 0;

    while (window.isOpen()) {
		sf::Clock deltaTimer;
		tanks[turn].startTurn();

		// --------------------------------------------------
		// GAMEPLAY LOOP
		// --------------------------------------------------

		bool eBrake = true;

		while (!tanks[turn].controls(deltaTimer.getElapsedTime().asMilliseconds()) && window.isOpen() && eBrake) {
			deltaTimer.restart();
			timer.restart();
			// Executes until the player shoots.

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			if (lastMouseState != sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lastMouseState && sf::Mouse::getPosition(window).x < SAND_SYSTEM_X && sf::Mouse::getPosition(window).x > 0) {
				sf::Vector2i position = sf::Mouse::getPosition(window);
				sand.detonate(Vector2D(position.x,SAND_SYSTEM_Y-position.y), 100, 50, explosionType::circular);
				eBrake = false;
			}

			lastMouseState = sf::Mouse::isButtonPressed(sf::Mouse::Left);


			window.clear();
			
			timer.restart();
			sand.render();
			//std::cout << "Sand render took " << timer.getElapsedTime().asMicroseconds() << " microseconds.\n";

			for (int i = 0; i < tanks.size(); i ++) {
				tanks[i].render(window, i == turn);
			}

			window.display();
			// TODO: Add frame rate lock.
		}
		
		projectiles.push_back(tanks[turn].result);

		turn = (turn+1)%tanks.size();

		std::cout << "Switching to other mode! Projectiles: " << projectiles.size() << '\n';

		int updateResult = 1;

		// --------------------------------------------------
		// UPDATE LOOP
		// No player input!
		// --------------------------------------------------
		bool tanksUpdated = true;

		while ((sand.update(Vector2D(0,-1)) || projectiles.size() > 0 || tanksUpdated) && window.isOpen()) {
			//std::cout << "UPDATE took " << timer.getElapsedTime().asMilliseconds() << " milliss.\n";
			tanksUpdated = false;
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
				tanksUpdated = tanksUpdated || tanks[i].update(&sand);
				tanks[i].render(window, false);
				if (tanks[i].health <= 0) {
					tanks.erase(tanks.begin() + i);
					i--;
					if (turn >= i) turn = (turn-1)%tanks.size();  
				}
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
