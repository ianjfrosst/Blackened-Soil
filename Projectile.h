#pragma once

#include "Vector2D.h"
#include "ExplosionType.h"
#include "Sand.h"
#include "Weapon.h"
#include "Explosion.h"

#define EXPL_DET 1
#define EXPL_OOB 2


struct Projectile {
	Vector2D pos;
	Vector2D vel;	// Un-normalized.

	Weapon * weap;

	sf::Clock birth;
    
	std::vector<sf::Vector2f> trace;

	// Needs to receive access to all players on the field to allow for damage.
	void detonate();

	sf::Vector2u getInverse(sf::Vector2u in);

	Projectile () {
		weap = new Weapon();
	}

	Projectile (Vector2D p, Vector2D v) {
		pos = p;
		vel = v;
		weap = new Weapon();
	}

	explosion result();

	// Renders the projectile, modifies the trace, and draws the trace.
	void render(sf::RenderWindow &window);
	
	// Moves the projectile along it's path, and calculates hits.
	// BIT 0 = explosion.
	// BIT 1 = dead.
	int update(sandSystem * map,  Vector2D influence);

};


