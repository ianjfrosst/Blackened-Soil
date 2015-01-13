#pragma once

#include "Vector2D.h"
#include "ExplosionType.h"
#include "Sand.h"
#include "Weapon.h"


class Projectile {
	Vector2D pos;
	Vector2D vel;	// Un-normalized.

	Weapon * weap;

	bool alive;

	std::vector<sf::Vector2f> trace;

	// Needs to receive access to all players on the field to allow for damage.
	void detonate();

	sf::Vector2u getInverse(sf::Vector2u in);

public :
	Projectile (Vector2D p, Vector2D v) {
		pos = p;
		vel = v;
		alive = true;
	}

	// Renders the projectile, modifies the trace, and draws the trace.
	void render(sf::RenderWindow &window);
	
	// Moves the projectile along it's path, and calculates hits.
	void update(sandSystem & map,  Vector2D influence);

};

