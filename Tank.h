#include "Vector2D.h"
#include "Projectile.h"
#include "SFML/Window.hpp"
#include <SFML/Window/Keyboard.hpp>


class Tank {
	Vector2D pos;		// Position, in world-space-pixels
	double rot;			// Rotation in RADIANS.

	float angle;
	int power;

	sf::Clock minTurn;

public :
	Projectile result;

	Tank() {
		pos = Vector2D();
		rot = 0;
		result = Projectile();

		angle = 0;
		power = 0;
	}

	void startTurn();

	void setPos(Vector2D p);
	void render(sf::RenderWindow &window, bool isMyTurn);
	void update();
	bool controls(int deltaMillis);
	void netUpdate();
	void netBroadcast();
};
