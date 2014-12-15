#include "World.h"

void World::create() {
	map.populate(150.0, 0.45);
	tanks = std::vector<Tank>();
}

sandSystem World::getMap() {
	return map;
}

void World::update() {
	map.update(Vector2D(0,-0.01));
}

void World::render(sf::RenderWindow & window) {
	map.render(window, Vector2D(0,0));
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i].render(window);
	}
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].render(window);
	}
}