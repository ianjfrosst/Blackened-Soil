#pragma once

#include "Vector2D.h"
#include "SandExtras.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
<<<<<<< Updated upstream
#include <thread>		// For multithreading things, because using OpenCL would be a pain in the ass.
=======
#include <mutex>
#include <list>
>>>>>>> Stashed changes

#define SAND_SYSTEM_X 350
#define SAND_SYSTEM_Y 350

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
<<<<<<< Updated upstream
	std::vector<sandPart> activeSandParts;
=======
	std::list<sandPart> activeSandParts;
	explosionData ed;
	std::mutex tMutex;
	

>>>>>>> Stashed changes
public :
	sandSystem() {}

	typedef std::list<sandPart>::iterator iter;

	void genHeight_recur(std::vector<double> &vec, int i, int j, double range, double smooth);
	std::vector<double> genHeightMap(int width, double range, double smooth);

	void render(sf::RenderWindow &window, Vector2D scrollPos);
	void populate(double range, double smooth);
	void update(Vector2D grav);
	void detonate(Vector2D loc, double power, double range);
	void affixSand(sandPart &i);
	void detachSand(int x, int y, Vector2D vel);
	void createSand(int x, int y, sf::Color c);
<<<<<<< Updated upstream
	void detonateThread (int startX, int endX, Vector2D loc, double power, double range);
=======

	//void detonateThread (int startX, int endX, Vector2D loc, double power, double range);

>>>>>>> Stashed changes
};
