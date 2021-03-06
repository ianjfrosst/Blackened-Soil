#pragma once

#include "Vector2D.h"
#include "Projectile.h"
#include "Explosion.h"
#include "Player.h"
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>

#define MAX_HEALTH 1000.0

class Tank {
	double rot;			// Rotation in RADIANS.

	float angle;
	int power;

	sf::Clock minTurn;

	int weaponSelection;

	Player * player;

    bool lastChangeKey;

public :
	float health;
	Projectile result;
	Vector2D pos;		// Position, in world-space-pixels
	Vector2D vel;		// Trust me on this one.
	int playerNumber;
	
	sf::Color col;

	Tank() {
		pos = Vector2D();
		rot = 0;
		result = Projectile();

		angle = 0;
		power = 20;
		weaponSelection = 0;
	}

	Tank(int Health, Player * p, int i) {
		pos = Vector2D();
		rot = 0;
		result = Projectile();

		player = p;
		playerNumber = i;

		angle = 0;
		power = 0;
		health = Health;
		weaponSelection = 0;
        lastChangeKey = false;
	}

	void startTurn(Weapon *);

	// Returns survival.
	bool takeDamage(explosion);

	sf::Vector2f transformPoint(Vector2D);

	void setPos(Vector2D p);
	void render(sf::RenderWindow &window, bool isMyTurn);

	// Returns whether the tank is still moving.
	bool update(sandSystem * sand);

	// Returns whether the tank fired.
	int controls(int deltaMillis, Weapon *,bool useAmmo = true);

	// Returns whether the projectile has been destroyed.
	bool checkProjectile(Projectile*);
};
