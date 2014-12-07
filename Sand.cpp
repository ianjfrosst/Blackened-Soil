#include "Sand.h"

void sandSystem::populate() {
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		int j = 0;
		for (j = 0; j < SAND_SYSTEM_Y; ++j) {
			if (j < 50) staticSand[i][j] = sf::Color(255, j%256, i%256,255);
			else staticSand[i][j] = sf::Color::Transparent;
		}
	}
	//std::cout << vector<sandPart>.max_size;
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		
		sandPart s;
		s.pos = Vector2D(i, 150);
		s.vel = Vector2D(0,20);
		s.col = sf::Color::Red;
		activeSandParts.push_back(s);
	}

}

void sandSystem::update (Vector2D grav) {
	std::cout << activeSandParts.size() << '\n';
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