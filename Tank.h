#include "Vector2D.h"
#include "SFML/Window.hpp"

class Tank {
	Vector2D pos;		// Position, in world-space-pixels
	Vector2D vel;		// Velocity, in world-space-pixels/second
	double rot;			// Rotation in RADIANS.

public :
	void render(sf::RenderWindow &window);
	void update();
	void controls();
	void netUpdate();
	void netBroadcast();
};
