#include "Projectile.h"

void Projectile::render(sf::RenderWindow &window) {
	if (trace.size() > 1) for (int i = 1; i < trace.size(); i++) {
		sf::Vertex line[] = {
			sf::Vertex(trace[i]),
			sf::Vertex(trace[i-1])
		};
		window.draw(line, 2, sf::Lines);
	}
}

bool Projectile::update(sandSystem * world, Vector2D influence) {
	trace.push_back(sf::Vector2f(pos.x, SAND_SYSTEM_Y-pos.y));

	if (pos.y < SAND_SYSTEM_Y && pos.y > 0 && pos.x > 0 && pos.x < SAND_SYSTEM_X) {
		if (world->staticSand[(int)pos.x][(int)pos.y] != sf::Color::Transparent) {
			world->detonate(Vector2D(pos.x,pos.y), 20, 50, explosionType::circular);
			// TODO: Apply damage here.
			alive = false;
		}
	}
	if (pos.x < 0 || pos.x > SAND_SYSTEM_X || pos.y < 0) alive = false;
	
	pos += vel;
	vel += influence;
	return alive;
}

