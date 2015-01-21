#pragma once

#include "Vector2D.h"
#include "Weapon.h"

struct explosion {
	bool didExplode;
	Vector2D pos;
	float size;
	float maxDMG;

	Weapon * weap;

	explosion (Vector2D p, int s, int m, Weapon*w) {
		didExplode = true;
		size = s;
		maxDMG = m;
		weap = w;
		pos = p;
	}
	explosion () {
		didExplode = false;
	}
};