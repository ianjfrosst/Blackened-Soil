#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>

class GameObject {
	Vector2D pos;		// Position in world-space-pixels
	Vector2D vel;		// Velocity in world-space-pixels/second
	double rot;			// Rotation in RADIANS.

public:
	GameObject() {}

    virtual void update();
	virtual void render(sf::RenderWindow &window);
};

