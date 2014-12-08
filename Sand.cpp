#include "Sand.h"

bool isPow2(int val) {
	return (val != 0) && ((val & (val - 1)) == 0);
}

int goUp(int val) {
	int size = 2;
	while (size + 1 < val) size *= 2;
	return size + 1;
}
std::vector<int> sandSystem::genHeightMap(int width, double smooth, double seed) {
	smooth = std::max(std::min(1.0, smooth), 0.0);

	int i;
	int size = isPow2(width - 1) ? width : goUp(width);
	std::vector<int> heightmap(size, 0);

	srand(seed);

	heightmap[0] = rand();

	return heightmap;
}

void sandSystem::populate(int sandHeight) {
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int j = 0; j < SAND_SYSTEM_Y; ++j) {
			if (j < sandHeight) {
				if (j < sandHeight - 10) {
					if (j < sandHeight - 30) staticSand[i][j] = sf::Color(140, 110, 60, 255);
					else staticSand[i][j] = (rand() % (sandHeight - j) > 10) ?
						sf::Color(140, 110, 60, 255) :
						sf::Color(60, 140+(rand()%100), 60, 255);	// Brown or a gradient

				} else staticSand[i][j] = sf::Color(60, 140+(rand()%100), 60, 255);			// Two different shades of green.
			}
			else staticSand[i][j] = sf::Color::Transparent;
		}
	}
}

void sandSystem::update(Vector2D grav) {
	//std::cout << activeSandParts.size() << '\n';
	for (int i = 0; i < activeSandParts.size(); i++) {
		activeSandParts[i].pos += activeSandParts[i].vel;
		activeSandParts[i].vel += grav;

		// Is the sand on the screen?
		if (activeSandParts[i].pos.x < SAND_SYSTEM_X && activeSandParts[i].pos.y < SAND_SYSTEM_Y &&
					activeSandParts[i].pos.x > 0 && activeSandParts[i].pos.y > 0) {

			if (staticSand[(int)activeSandParts[i].pos.x][(int)activeSandParts[i].pos.y] != sf::Color::Transparent) {
				affixSand(&i);
				continue;
			}

		} else affixSand(&i);
		/*
		if (activeSandParts[i].pos.x <= 0) {
			activeSandParts[i].pos.x = 0;
			activeSandParts[i].vel.x = 0;
		}
		if (activeSandParts[i].pos.y <= 0) {
			affixSand(&i);
			continue;
		}
		if (activeSandParts[i].pos.x >= SAND_SYSTEM_X) {
			activeSandParts[i].pos.x = SAND_SYSTEM_X-1;
			activeSandParts[i].vel.x = 0;
		}
		if (activeSandParts[i].pos.y >= SAND_SYSTEM_Y) {
			activeSandParts[i].pos.y = SAND_SYSTEM_Y-1;
			activeSandParts[i].vel.y = 0;
		}*/
	}
	for (int x = 0; x < 500; ++x) {
		for (int y = 1; y < 500; ++y) {
			if (staticSand[x][y-1] == sf::Color::Transparent && staticSand[x][y] != sf::Color::Transparent) {
				detachSand(x, y, Vector2D(0,0));
			}
		}
	}
}

/// <summary>
/// Gets the inverse square at power from src to (x,y).
/// Breaks if src and x,y are the same point (division by zero).
/// </summary>
/// <param name="x">The x coordinate of the target point. For sand grains.</param>
/// <param name="y">The y coordinate of the target point. For sand grains.</param>
Vector2D getInvSq(Vector2D src, int x, int y, double power) {
	// TODO: This is probably broken. Should be replaced by a linear power calc.
	Vector2D dirVec = (Vector2D(x,y) - src);
	return Vector2D(dirVec.GetDir(), power/(dirVec.GetSqrMag()+1));
}

/// <summary>
/// Creates an explosion of size "range" at "loc" with "power".
/// Power is the maximum (?) power that the explosion can apply.
/// </summary>
void sandSystem::detonate(Vector2D loc, double power, double range) {
	std::cout << "BOOM!";
	for (int x = loc.x-range < 0 ? 0 : loc.x-range; x < (loc.x+range > SAND_SYSTEM_X ? SAND_SYSTEM_X : loc.x+range); x++) {
		// The half that needs destroying.
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			double a = x - loc.x;
			double b = y - loc.y;
			if (a*a + b*b <= range*range) {
				if (y > loc.y) detachSand(x, y, getInvSq(loc, x, y, power));
				else staticSand[x][y] = sf::Color::Transparent;
			}
		}
	}
}

void sandSystem::affixSand(int * i) {
	//std::cout << "Affixing particle...\n";
	// The position is inside of the static sand, so insert the object and shift everything else up.
	int start = (int)activeSandParts[*i].pos.y;
	int o = start;

	for (;staticSand[(int)activeSandParts[*i].pos.x][o] != sf::Color::Transparent;++o);
	for (;o > start; --o)
		staticSand[(int)activeSandParts[*i].pos.x][o] = staticSand[(int)activeSandParts[*i].pos.x][o-1];

	staticSand[(int)activeSandParts[*i].pos.x][(int)activeSandParts[*i].pos.y] = activeSandParts[*i].col;

	activeSandParts.erase(activeSandParts.begin() + *i);
	--*i;
}

void sandSystem::detachSand(int x, int y, Vector2D vel) {
	sandPart sp;
	sp.pos = Vector2D(x, y);
	sp.col = staticSand[x][y];
	sp.vel = vel;
	activeSandParts.push_back(sp);
	staticSand[x][y] = sf::Color::Transparent;
}

void sandSystem::render(sf::RenderWindow &window, Vector2D scrollPos) {
	sf::Image out;
	out.create(SAND_SYSTEM_X,SAND_SYSTEM_Y);

	for (int o = 0; o < SAND_SYSTEM_Y; o++) {
		out.setPixel(0, o, sf::Color::Blue);
		out.setPixel(SAND_SYSTEM_Y-1, o, sf::Color::Blue);
	}

	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		out.setPixel(i, 0, sf::Color::Blue);
		out.setPixel(i, SAND_SYSTEM_Y-1, sf::Color::Blue);

		for (int o = 0; o < SAND_SYSTEM_Y; ++o) {
			out.setPixel(i, o, staticSand[i][o]);
		}
	}

	for (int i = 0; i < activeSandParts.size(); i++) {
		if (activeSandParts[i].pos.x < SAND_SYSTEM_X && activeSandParts[i].pos.y < SAND_SYSTEM_Y)
			out.setPixel((int)activeSandParts[i].pos.x, (int)activeSandParts[i].pos.y, activeSandParts[i].col);
	}


	// Since we want cartesian (0,0 = Lower-left) coordinates, we need to flip the pixel array before we render it.
	out.flipVertically();

	sf::Texture outTex;
	outTex.loadFromImage(out);
	sf::Sprite outSpr;
	outSpr.setTexture(outTex, true);
	outSpr.setPosition((sf::Vector2f)scrollPos);
	window.draw(outSpr);
}
