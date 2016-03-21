#include "Sand.h"

sf::Color& sandPile::operator[](const int i) {
	return data[i];
}

// generates a random double
double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

// gets the midpoint of two values
inline double getMid(double a, double b) {
	return (a+b)/2;
}

// is val a power of two?
bool isPow2(int val) {
	return (val != 0) && ((val & (val - 1)) == 0);
}

// return the next highest power of two + 1
int goUp(int val) {
	int size = 2;
	while (size + 1 < val) size *= 2;
	return size + 1;
}

// vec, i and j are for recursion.
// range is the maximum variance (up or down) for each midpoint.
// smooth is the decay constant. between 0  and 1.
void sandSystem::genHeight_recur(std::vector<double> &vec, int i, int j, double range, double smooth) {
	if (j-i == 1) return;
	else {
		int m = getMid(i, j);
		vec[m] = getMid(vec[i], vec[j]) + fRand(-range, range);
		range *= smooth;
		genHeight_recur(vec, i, m, range, smooth);
		genHeight_recur(vec, m, j, range, smooth);
	}
}

std::vector<double> sandSystem::genHeightMap(int width, double range, double smooth) {
	int size = isPow2(width - 1) ? width : goUp(width);
	std::vector<double> hMap(size);
	smooth = std::max(std::min(1.0, smooth), 0.0);
    int minS = 0.30 * SAND_SYSTEM_Y;
    int maxS = 0.60 * SAND_SYSTEM_Y;
	hMap[0] = fRand(minS, maxS);
	hMap[hMap.size() - 1] = fRand(minS, maxS);
	genHeight_recur(hMap, 0, hMap.size() - 1, range, smooth);
	return hMap;
}

void sandSystem::populate(double range, double smooth) {
	std::vector<double> hMap = genHeightMap(SAND_SYSTEM_X, range, smooth);
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int j = 0; j < SAND_SYSTEM_Y; ++j) {
			if (j < hMap[i]) {
				if (j < hMap[i] - 10) {
					if (j < hMap[i] - 30) staticSand[i][j] = sf::Color(140, 110, 60, 255);
					else staticSand[i][j] = (rand() % (int)(hMap[i] - j) > 10) ?
						sf::Color(140, 110, 60, 255) :
						sf::Color(60, 140+(rand()%100), 60, 255);	// Brown or a gradient
				} else staticSand[i][j] = sf::Color(60, 140+(rand()%100), 60, 255);			// Two different shades of green.
			} else staticSand[i][j] = sf::Color::Transparent;
		}
	}
}

bool sandSystem::update(Vector2D grav) {
	sf::Clock updateClock;

	for (int i = 0; i < activeSandParts.size(); i++) {
		activeSandParts[i].pos += activeSandParts[i].vel;
		activeSandParts[i].vel += grav;
		activeSandParts[i].vel *= 0.999;

		// Is the sand on the screen?
		if (activeSandParts[i].pos.x < SAND_SYSTEM_X && activeSandParts[i].pos.y < SAND_SYSTEM_Y &&
				activeSandParts[i].pos.x > 0) {

			if (activeSandParts[i].pos.y < 0) {
				activeSandParts[i].pos.y = 0;
                staticSand[(int)activeSandParts[i].pos.x].flag = true;
                affixSand(i);
				continue;
			}

			if (staticSand[(int) activeSandParts[i].pos.x][(int) activeSandParts[i].pos.y] != sf::Color::Transparent) {
                staticSand[(int)activeSandParts[i].pos.x].flag = true;
                affixSand(i);
                continue;
			}
		} else activeSandParts.erase(activeSandParts.begin() + i);
	}

	//if (activeSandParts.size() > 0) std::cout << "ACTIVE UPDATE took " << updateClock.getElapsedTime().asMicroseconds() << " micros. "
	//	<< p << " particles.\n";

	updateClock.restart();

	for (int x = 0; x < SAND_SYSTEM_X; ++x) {
        if (staticSand[x].flag) {
            staticSand[x].flag = false;
            for (int y = 1; y < SAND_SYSTEM_Y; y++) {
                if (staticSand[x][y] != sf::Color::Transparent) {
                    if (staticSand[x][y - 1] == sf::Color::Transparent) {
                        staticSand[x].flag = true;
                        detachSand(x, y, Vector2D(0, 0), true);
                    }
                }
            }
        }
	}
	//std::cout << "STATIC UPDATE took " << updateClock.getElapsedTime().asMicroseconds() << " micros.\n";

	return activeSandParts.size() > 0;
}

/// <summary>
/// Gets the inverse square at power from src to (x,y).
/// Breaks if src and x,y are the same point (division by zero).
/// </summary>
/// <param name="x">The x coordinate of the target point. For sand grains.</param>
/// <param name="y">The y coordinate of the target point. For sand grains.</param>
Vector2D getInvSq(Vector2D src, int x, int y, double power) {
	Vector2D delta = (Vector2D(x,y) - src);
	return (delta/delta.GetMag())*(power/delta.GetSqrMag());
}

Vector2D getCalderaForce(Vector2D src, int x, int y, double power) {
	// TODO: This is probably broken. Should be replaced by a linear power calc.
	Vector2D dirVec = (Vector2D(x,y) - src);
	return Vector2D(dirVec.x, power/dirVec.GetSqrMag());
}

void sandSystem::detonate(Vector2D loc, double power, double range, explosionType type) {
	switch(type) {
	case explosionType::calderic:
		detonateCalderic(loc, power, range); 
		break;
	case explosionType::circular:
		detonateCircular(loc, power, range);
		break;
	case explosionType::disintegrative:
		detonateDisintegrate(loc, power, range);
		break;
	case explosionType::bunkerbuster:
		detonateBunkerbuster(loc, power, range);
		break;
	}
}

// TODO: need to have the velocity of the projectile.
void sandSystem::detonateBunkerbuster(Vector2D loc, double power, double range) {

}

/// <summary>
/// Creates an explosion of size "range" at "loc" with "power".
/// Power is the maximum (?) power that the explosion can apply.
/// </summary>
void sandSystem::detonateDisintegrate(Vector2D loc, double power, double range) {
	// Right (positive) side
	int RHScap = (loc.x+range > SAND_SYSTEM_X ? SAND_SYSTEM_X : loc.x+range);
	for (int x = loc.x; x < RHScap; x++) {
        staticSand[x].flag = true;
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			int a = x - loc.x;
			int b = y - loc.y;	// Also, we could probably cut out a few operations with a more imprecise calculation of circles.
			if (a*a + b*b <= range*range) {
				staticSand[x][y] = sf::Color::Transparent;
			}
		}
	}

	// Left (minus) side
	int LHScap = loc.x-range < 0 ? 0 : loc.x-range;
	for (int x = loc.x; x > LHScap; x--) {
        staticSand[x].flag = true;
        for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			int a = x - loc.x;
			int b = y - loc.y;
			if (a*a + b*b <= range*range) {
				staticSand[x][y] = sf::Color::Transparent;
			}
		}
	}
}

/// <summary>
/// Creates an explosion of size "range" at "loc" with "power".
/// Power is the maximum (?) power that the explosion can apply.
/// </summary>
void sandSystem::detonateCircular(Vector2D loc, double power, double range) {
	// Right (positive) side
	int RHScap = (loc.x+range > SAND_SYSTEM_X ? SAND_SYSTEM_X : loc.x+range);
	for (int x = loc.x; x < RHScap; x++) {
        staticSand[x].flag = true;
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			int a = x - loc.x;
			int b = y - loc.y;	// Also, we could probably cut out a few operations with a more imprecise calculation of circles.
			if (a*a + b*b <= range*range) {
				detachSand(x, y, getInvSq(loc, x, y, range*power));
			}
		}
	}

	// Left (minus) side
	int LHScap = loc.x-range < 0 ? 0 : loc.x-range;
	for (int x = loc.x; x > LHScap; x--) {
        staticSand[x].flag = true;
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			int a = x - loc.x;
			int b = y - loc.y;
			if (a*a + b*b <= range*range) {
				detachSand(x, y, getInvSq(loc, x, y, range*power));
			}
		}
	}
}


/// <summary>
/// Creates an explosion of size "range" at "loc" with "power".
/// Power is the maximum (?) power that the explosion can apply.
/// </summary>
void sandSystem::detonateCalderic(Vector2D loc, double power, double range) {
	// Right (positive) side
	int RHScap = (loc.x+range > SAND_SYSTEM_X ? SAND_SYSTEM_X : loc.x+range);
	for (int x = loc.x; x < RHScap; x++) {
        staticSand[x].flag = true;
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			int a = x - loc.x;
			int b = y - loc.y;	// Also, we could probably cut out a few operations with a more imprecise calculation of circles.
			if (a*a + b*b <= range*range) {
				detachSand(x, y, getCalderaForce(loc, x, y, range*power));
			}
		}
	}

	// Left (minus) side
	int LHScap = loc.x-range < 0 ? 0 : loc.x-range;
	for (int x = loc.x; x > LHScap; x--) {
        staticSand[x].flag = true;
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			int a = x - loc.x;
			int b = y - loc.y;
			if (a*a + b*b <= range*range) {
				detachSand(x, y, getCalderaForce(loc, x, y, range*power));
			}
		}
	}
}

void sandSystem::affixSand(int &i) {
	//std::cout << "Affixing particle...\n";
	// The position is inside of the static sand, so insert the object and shift everything else up.
	Vector2D v = activeSandParts[i].vel;
	Vector2D p = activeSandParts[i].pos;

	v.SetDM(v.GetDir(), 1);

	// TODO: Ian, this while loop is horrifying. It's also your problem.

	while (p.x-v.x > 0 && p.x-v.x < SAND_SYSTEM_X && p.y-v.y > 0 && p.y-v.y < SAND_SYSTEM_Y &&
			staticSand[(int)p.x][(int)p.y] != sf::Color::Transparent) {
		p -= v;
	}

	if (p.y > 0 && p.x < SAND_SYSTEM_X && p.x > 0) staticSand[(int)p.x][(int)p.y] = activeSandParts[i].col;

	activeSandParts.erase(activeSandParts.begin() + i);
	--i;
}

void sandSystem::detachSand(int x, int y, Vector2D vel, bool force) {
	if ((vel.GetSqrMag() >= 1 || force) && staticSand[x][y] != sf::Color::Transparent) {
	//if (staticSand[x][y] != sf::Color::Transparent) {
		/*if (vel.GetSqrMag() <= 1) staticSand[x][y] = sf::Color::Red;
		else staticSand[x][y] = sf::Color::;*/
		sandPart sp;
		sp.pos = Vector2D(x, y);
		sp.col = staticSand[x][y];
		sp.vel = vel;
		activeSandParts.push_back(sp);
		staticSand[x][y] = sf::Color::Transparent;
	}
}

void sandSystem::render() {
	sf::Image out;
	out.create(SAND_SYSTEM_X,SAND_SYSTEM_Y);


    // From here
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int o = 0; o < SAND_SYSTEM_Y; ++o) {
			out.setPixel(i, o, staticSand[i][o]);
		}
	}

	for (int i = 0; i < activeSandParts.size(); i++) {
		if (activeSandParts[i].pos.x < SAND_SYSTEM_X && activeSandParts[i].pos.y < SAND_SYSTEM_Y)
			out.setPixel((int)activeSandParts[i].pos.x, (int)activeSandParts[i].pos.y, activeSandParts[i].col);
	}
	// to here is ~2.5 ms

	// Since we want cartesian (0,0 = Lower-left) coordinates, we need to flip the pixel array before we render it.
	out.flipVertically(); // Takes about 3 ms

	sf::Texture outTex;
	outTex.loadFromImage(out);
	sf::Sprite outSpr;
	outSpr.setTexture(outTex, true);
	outSpr.setPosition(sf::Vector2f(0,0));
	window->draw(outSpr);
}




