#include "World.h"


void World::render(sf::RenderWindow window) {
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i].render(window, false);
	}
	for (int i = 0; i < projectiles.size(); i++) {
		//projectiles[i].render(window);
	}
}