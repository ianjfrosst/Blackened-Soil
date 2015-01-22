#pragma once

#include "Vector2D.h"
#include "ExplosionType.h"
#include "Sand.h"
#include "Weapon.h"
#include "Explosion.h"

#define EXPL_DET 1	// The projectile has exploded.
#define EXPL_OOB 2	// The projectile has gone out of bounds.
#define EXPL_SPL 4	// The projectile has split.
#define EXPL_SAD 8	// The projectile has split and died.


struct Projectile {
	Vector2D pos;
	Vector2D vel;	// Un-normalized.

	bool splitting;

	Weapon * weap;

	sf::Clock birth;
    
	std::vector<sf::Vector2f> trace;

	// Needs to receive access to all players on the field to allow for damage.
	void detonate();

	sf::Vector2u getInverse(sf::Vector2u in);

	Projectile () {
		splitting = false;
		weap = new Weapon();
	}

	Projectile (Vector2D p, Vector2D v) {
		splitting = false;
		pos = p;
		vel = v;
		weap = new Weapon();
	}

	Projectile (Vector2D p, Vector2D v, Weapon * weapon) {
		splitting = false;
		pos = p;
		vel = v;
		weap = weapon;
	}

	explosion result();

	// Renders the projectile, modifies the trace, and draws the trace.
	void render(sf::RenderWindow &window);

	// Returns a vector of projectiles upon splitting.
	std::vector<Projectile> split ();
	
	// Moves the projectile along it's path, and calculates hits.
	// BIT 0 = explosion.
	// BIT 1 = dead.
	int update(sandSystem * map,  Vector2D influence);

};


