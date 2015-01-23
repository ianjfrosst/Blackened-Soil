#pragma once

#include "Vector2D.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>		// For multithreading things, because using OpenCL would be a pain in the ass.

#include "ExplosionType.h"

#define SAND_SYSTEM_X 500
#define SAND_SYSTEM_Y 500


struct sandPart {
	Vector2D pos;
	Vector2D vel;
	sf::Color col;

	sandPart() {}
	sandPart(Vector2D p, Vector2D v, sf::Color c) : pos(p), vel(v), col(c) {}
};

struct sandPile {
	sf::Color data[SAND_SYSTEM_Y];
	bool flag;

	sandPile() {
		flag = false;
	}

	sf::Color& operator[](const int i);
};

class sandSystem {
	std::vector<sandPart> activeSandParts;
	sf::RenderWindow * window;
public :
	sandPile * staticSand;

	sandSystem() {}
	sandSystem(sf::RenderWindow * w) {
		window = w; 
        staticSand = new sandPile[SAND_SYSTEM_X];
	}

	void genHeight_recur(std::vector<double> &vec, int i, int j, double range, double smooth);
	std::vector<double> genHeightMap(int width, double range, double smooth);

	void render();
	void populate(double range, double smooth);

	// Return whether it needs to continue.
	bool update(Vector2D grav);

	void detonate(Vector2D loc, double power, double range, explosionType type);

	void detonateCircular(Vector2D loc, double power, double range);
	void detonateCalderic(Vector2D loc, double power, double range);
	void detonateDisintegrate(Vector2D loc, double power, double range);
	void detonateBunkerbuster(Vector2D loc, double power, double range);

	void affixSand(int &i);
	void detachSand(int x, int y, Vector2D vel, bool force = false);
};
