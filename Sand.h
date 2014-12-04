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
	// A nullptr signals the top of the pile.
	sf::Color staticSand[SAND_SYSTEM_X][SAND_SYSTEM_Y];
	std::vector<sandPart> activeSandParts;



public :
	void render (sf::RenderWindow, Vector2D scrollPos);
	void populate ();
};
