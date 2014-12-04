#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>
#include <stack>

#define SAND_SYSTEM_X 1920
#define SAND_SYSTEM_Y 1080

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
};
