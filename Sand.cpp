#include "Sand.h"

void sandSystem::populate() {
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int o = 0; o < SAND_SYSTEM_Y; ++o) {
			 staticSand[i][o].r = i % 255;
			 staticSand[i][o].b = o % 255;
		}
	}
}

void sandSystem::render(sf::RenderWindow window, Vector2D scrollPos) {
	sf::Image out;
	out.create(SAND_SYSTEM_X,SAND_SYSTEM_Y);
	for (int i = 0; i < SAND_SYSTEM_X; ++i) {
		for (int o = 0; o < SAND_SYSTEM_Y && staticSand[i][o].a == 255; ++o) {
			out.setPixel(i, o, staticSand[i][o]);
		}
	}

	for (sandPart i : activeSandParts) {
		out.setPixel(i.pos.x, i.pos.y, i.col);
	}

	sf::Texture outTex;
	outTex.loadFromImage(out);
	sf::Sprite outSpr;
	outSpr.setTexture(outTex, true);
	outSpr.setPosition((sf::Vector2f)scrollPos);
	window.draw(outSpr);
}