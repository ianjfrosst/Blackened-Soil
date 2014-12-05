#include "Sand.h"

void sandSystem::populate() {
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int o = SAND_SYSTEM_Y; o > SAND_SYSTEM_Y/4; --o) {
			staticSand[i][o] = sf::Color::Red;
		}
	}

	for (int i = 0; i < SAND_SYSTEM_X; ++i) {

	}

}

void sandSystem::update () {
	for (int i = 0; i < activeSandParts.size(); i++) {
		activeSandParts[i].pos += activeSandParts[i].vel;
	}
}

void sandSystem::render(sf::RenderWindow &window, Vector2D scrollPos) {
	sf::Image out;
	out.create(SAND_SYSTEM_X,SAND_SYSTEM_Y);
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int o = 0; o < SAND_SYSTEM_Y && staticSand[i][o].a == 255; ++o) {
			out.setPixel(i, o, staticSand[i][o]);
		}
	}

	/*for (int i = 0; i < activeSandParts.size(); i++) {
		out.setPixel(activeSandParts[i].pos.x, activeSandParts[i].pos.y, activeSandParts[i].col);
	}*/

	sf::Texture outTex;
	outTex.loadFromImage(out);
	sf::Sprite outSpr;
	outSpr.setTexture(outTex, true);
	outSpr.setPosition((sf::Vector2f)scrollPos);
	window.draw(outSpr);
}