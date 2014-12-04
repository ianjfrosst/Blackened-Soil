#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>
#include <vector>

#define SAND_SYSTEM_X 200
#define SAND_SYSTEM_Y 200

struct sandPart {
public :
	Vector2D pos;
	Vector2D vel;
	sf::Color col;
};

class sandSystem {
	sf::Color staticSand[SAND_SYSTEM_X][SAND_SYSTEM_Y];
	std::vector<sandPart> activeSandParts;
public :
	sandSystem(){}
	void render (sf::RenderWindow &window, Vector2D scrollPos);
	void populate ();
	void update ();
};
