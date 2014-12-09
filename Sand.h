#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>		// For multithreading things, because using OpenCL would be a pain in the ass.

#define SAND_SYSTEM_X 500
#define SAND_SYSTEM_Y 500

struct sandPart {
public :
	Vector2D pos;
	Vector2D vel;
	sf::Color col;

	sandPart() {}
	sandPart(Vector2D p, Vector2D v, sf::Color c) : pos(p), vel(v), col(c) {}
};

class sandSystem {
	sf::Color staticSand[SAND_SYSTEM_X][SAND_SYSTEM_Y];
	std::vector<sandPart> activeSandParts;
public :
	sandSystem() {}

	void genHeightMap(std::vector<double> &vec, int i, int j, double range, double smooth);

	void render(sf::RenderWindow &window, Vector2D scrollPos);
	void populate(double range, double smooth);
	void update(Vector2D grav);
	void detonate(Vector2D loc, double power, double range);
	void affixSand(int * i);
	void detachSand(int x, int y, Vector2D vel);
	void detonateThread (int startX, int endX, Vector2D loc, double power, double range);
};
