#include "Sand.h"

void sandSystem::populate(int sandHeight) {
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int j = 0; j < SAND_SYSTEM_Y; ++j) {
			if (j < sandHeight) staticSand[i][j] = sf::Color::Cyan;//sf::Color(255, j%256, i%256, 255);
			else staticSand[i][j] = sf::Color::Transparent;
		}
	}
}

void sandSystem::update(Vector2D grav) {
	//std::cout << activeSandParts.size() << '\n';
	for (int i = 0; i < activeSandParts.size(); i++) {
		activeSandParts[i].pos += activeSandParts[i].vel;
		activeSandParts[i].vel += grav;

		//std::cout << activeSandParts[i].pos << '\n';
		if (activeSandParts[i].pos.x < SAND_SYSTEM_X && activeSandParts[i].pos.y < SAND_SYSTEM_Y &&
					activeSandParts[i].pos.x > 0 && activeSandParts[i].pos.y > 0) {

			if (staticSand[(int)activeSandParts[i].pos.x][(int)activeSandParts[i].pos.y] != sf::Color::Transparent) {
				affixSand(&i);
				continue;
			}

		}
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
		}
	}
}

/// <summary>
/// Gets the inverse square at power from src to (x,y).
/// Breaks if src and x,y are the same point (division by zero).
/// </summary>
/// <param name="x">The x coordinate of the target point. For sand grains.</param>
/// <param name="y">The y coordinate of the target point. For sand grains.</param>
Vector2D getInvSq(Vector2D src, int x, int y, int power) {
	// TODO: This is probably broken. Should be replaced by a linear power calc.
	Vector2D dirVec = (Vector2D(x,y) - src);
	return Vector2D(dirVec.GetDir(), power/(dirVec.GetSqrMag()+1));
}

/// <summary>
/// Creates an explosion of size "range" at "loc" with "power".
/// Power is the maximum (?) power that the explosion can apply.
/// </summary>
void sandSystem::detonate(Vector2D loc, float power, float range) {
	std::cout << "BOOM!";
	for (int x = loc.x-range < 0 ? 0 : loc.x-range; x < (loc.x+range > SAND_SYSTEM_X ? SAND_SYSTEM_X : loc.x+range); x++) {
		// The half that needs destroying.
		for (int y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			double a = x - loc.x;
			double b = y - loc.y;
			if (a*a + b*b <= range*range) {
				if (y > loc.y) {
					sandPart sp;
					sp.pos = Vector2D(x, y);
					sp.col = sf::Color::Yellow;//  staticSand[x][y];
					sp.vel = getInvSq(loc, x, y, power);
					activeSandParts.push_back(sp);
				}
				staticSand[x][y] = sf::Color::Transparent;
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


