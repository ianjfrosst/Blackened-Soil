#include "Vector2D.h"
#include "Projectile.h"
#include "Explosion.h"
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>

#define MAX_HEALTH 1000.0

class Tank {
	double rot;			// Rotation in RADIANS.

	float angle;
	int power;

	sf::Clock minTurn;

public :
	float health;
	Projectile result;
	Vector2D pos;		// Position, in world-space-pixels
	Vector2D vel;		// Trust me on this one.
	int playerNumber;

	Tank() {
		pos = Vector2D();
		rot = 0;
		result = Projectile();

		angle = 0;
		power = 0;
	}

	Tank(int Health, int player) {
		pos = Vector2D();
		rot = 0;
		result = Projectile();

		playerNumber = player;

		angle = 0;
		power = 0;
		health = Health;
	}

	void startTurn();

	// Returns survival.
	bool takeDamage(explosion);

	sf::Vector2f transformPoint(Vector2D);

	void setPos(Vector2D p);
	void render(sf::RenderWindow &window, bool isMyTurn);
	bool update(sandSystem * sand);
	bool controls(int deltaMillis);
	void netUpdate();
	void netBroadcast();
};
