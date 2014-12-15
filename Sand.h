#pragma once

#include "Vector2D.h"
#include "SandExtras.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <mutex>

#define MAX_THREADS 4


class sandSystem {
	sf::Color staticSand[SAND_SYSTEM_X][SAND_SYSTEM_Y];
	std::list<sandPart> activeSandParts;
	explosionData ed;
	std::mutex tMutex;

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

	void detonateThread ();
	//void detonateThread (int startX, int endX, Vector2D loc, double power, double range);

};

