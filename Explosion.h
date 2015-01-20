#pragma once

#include "Vector2D.h"

struct explosion {
	bool didExplode;
	Vector2D pos;
	int size;
	int maxDMG;

	explosion (Vector2D pos, int s, int m) {
		didExplode = true;
		size = s;
		maxDMG = m;
	}
	explosion () {
		didExplode = false;
	}
};