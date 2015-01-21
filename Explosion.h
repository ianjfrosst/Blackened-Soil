#pragma once

#include "Vector2D.h"
#include "Weapon.h"

struct explosion {
	bool didExplode;
	Vector2D pos;
	int size;
	int maxDMG;

	Weapon * weap;

	explosion (Vector2D pos, int s, int m, Weapon*w) {
		didExplode = true;
		size = s;
		maxDMG = m;
		weap = w;
	}
	explosion () {
		didExplode = false;
	}
};