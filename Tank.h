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
		weaponSelection = 0;
	}

	Tank(int Health) {
		pos = Vector2D();
		rot = 0;
		result = Projectile();

		angle = 0;
		power = 0;
		health = Health;
		weaponSelection = 0;
	}

	void startTurn();

	// Returns survival.
	bool takeDamage(explosion);

	sf::Vector2f transformPoint(Vector2D);

	void setPos(Vector2D p);
	void render(sf::RenderWindow &window, bool isMyTurn);

	// Returns whether the tank is still moving.
	bool update(sandSystem * sand);

	// Returns index of weapon fired in the weapons array.
	int controls(int deltaMillis, Weapon *);

	void netUpdate();
	void netBroadcast();
};
