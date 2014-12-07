#include "Sand.h"

void sandSystem::populate(int sandHeight) {
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int j = 0; j < SAND_SYSTEM_Y; ++j) {
			if (j < sandHeight) staticSand[i][j] = sf::Color(255, j%256, i%256, 255);
			else staticSand[i][j] = sf::Color::Transparent;
		}
	}
	//std::cout << vector<sandPart>.max_size;
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		activeSandParts.push_back(sandPart(Vector2D(i, 150), Vector2D(0,20), sf::Color::Red));
	}

}

void sandSystem::update(Vector2D grav) {
	//std::cout << activeSandParts.size() << '\n';
	for (int i = 0; i < activeSandParts.size(); i++) {
		activeSandParts[i].pos += activeSandParts[i].vel;
		activeSandParts[i].vel += grav;

		//std::cout << activeSandParts[i].pos << '\n';
		if (activeSandParts[i].pos.x < SAND_SYSTEM_X && activeSandParts[i].pos.y < SAND_SYSTEM_Y &&
			activeSandParts[i].pos.x > 0 && activeSandParts[i].pos.y > 0
			) {

			//std::cout << "Particle in range...\n";
			if (staticSand[(int)activeSandParts[i].pos.x][(int)activeSandParts[i].pos.y] != sf::Color::Transparent) {
				//std::cout << "Affixing particle...\n";
				// The position is inside of the static sand, so insert the object and shift everything else up.
				int start = (int)activeSandParts[i].pos.y;
				int o = start;
				int end = 0; // Temporary thing...

				for (;staticSand[(int)activeSandParts[i].pos.x][o] != sf::Color::Transparent;++o);
				end = o;
				for (;o > start; --o)
					staticSand[(int)activeSandParts[i].pos.x][o] = staticSand[(int)activeSandParts[i].pos.x][o-1];

				staticSand[(int)activeSandParts[i].pos.x][(int)activeSandParts[i].pos.y] = activeSandParts[i].col;

				activeSandParts.erase(activeSandParts.begin() + i);
				--i;

			}
		}
		if (activeSandParts[i].pos.x <= 0) {
			activeSandParts[i].pos.x = 0;
			activeSandParts[i].vel.x = 0;
		}
		if (activeSandParts[i].pos.y <= 0) {
			activeSandParts[i].pos.y = 0;
			activeSandParts[i].vel.y = 0;
		}
		if (activeSandParts[i].pos.x >= SAND_SYSTEM_X) {
			activeSandParts[i].pos.x = 0;
			activeSandParts[i].vel.x = 0;
		}
		if (activeSandParts[i].pos.y >= SAND_SYSTEM_Y) {
			activeSandParts[i].pos.y = SAND_SYSTEM_Y-1;
			activeSandParts[i].vel.y = 0;
		}
	}

	//std::cout << "Reached end of update.\n";
}

/// <summary>
/// Gets the inverse square at power from src to (x,y).
/// Breaks if src and x,y are the same point (division by zero).
/// </summary>
/// <param name="x">The x coordinate of the target point. For sand grains.</param>
/// <param name="y">The y coordinate of the target point. For sand grains.</param>
Vector2D getInvSq(Vector2D src, int x, int y, int power) {
	// TODO: This is probably broken. Should be replaced by a linear power calc.
	return power/((src - Vector2D(x,y)).GetSqrMag()+1);
}

/// <summary>
/// Creates an explosion of size "range" at "loc" with "power".
/// Power is the maximum (?) power that the explosion can apply.
/// </summary>
void sandSystem::detonate(Vector2D loc, float power, float range) {
	std::cout << "BOOM!";
	for (int x = loc.x-range < 0 ? 0 : loc.x-range; x < (loc.x+range > SAND_SYSTEM_X ? SAND_SYSTEM_X : loc.x+range); x++) {
		int y;
		// The half that needs destroying.
		for (y = loc.y-range < 0 ? 0 : loc.y-range; y < (loc.y > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y); y++) {
			staticSand[x][y] = sf::Color::Green;
		}

		// The half that needs launching and destroying, selectively.
		for (; y < (loc.y+range > SAND_SYSTEM_Y ? SAND_SYSTEM_Y : loc.y+range); y++) {
			staticSand[x][y] = sf::Color::Blue;
			sandPart sp;
			sp.pos = Vector2D(x,y);
			sp.col = staticSand[x][y];
			sp.vel = getInvSq(loc, x, y, power);
			activeSandParts.push_back(sp);
		}
	}
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

		for (int o = 0; o < SAND_SYSTEM_Y && staticSand[i][o].a != 0; ++o) {
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


