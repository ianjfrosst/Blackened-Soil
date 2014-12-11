#pragma once

#include "Sand.h"

/// <summary>
/// A list that only clears itself once per frame.
/// Of a static size, but a big one, in this case the number of sand grains on screen.
/// </summary>
class ParticleList {
	sandPart contents[SAND_SYSTEM_X*SAND_SYSTEM_Y];

	// top points to the cap entry on the array, which has alive = 2 so that iterators can finish.
	int top;
public :
	ParticleList(){
		top = 0;
		contents[0] = sandPart();
		contents[0].alive = 2;
	};

	sandPart &operator[](const int i);

	void add(sandPart);
	void erase(int);
	void clear();
	int size();
};
