#include "Sand.h"

/// <summary>
/// A list that only clears itself once per frame.
/// Of a static size, but a big one, in this case the number of sand grains on screen.
/// </summary>
class ParticleList {
	sandPart contents[SAND_SYSTEM_X*SAND_SYSTEM_Y];
public :
	ParticleList(){};

	void add(sandPart sp);
	void erase(int t);
	void clear();
};
