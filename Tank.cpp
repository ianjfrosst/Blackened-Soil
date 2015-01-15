#include "Tank.h"

void Tank::render(sf::RenderWindow &window, bool isMyTurn){
	sf::CircleShape shape = sf::CircleShape(5);
	shape.setPosition(pos.x, SAND_SYSTEM_Y-pos.y-10);
	if (isMyTurn) shape.setFillColor(sf::Color::Red);
	window.draw(shape);
}

void Tank::setPos(Vector2D p) {
	pos = p;
}

void Tank::update() {}

bool Tank::controls(int deltaMillis) {
	bool key_W = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool key_S = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool key_A = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool key_D = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

	bool fired = false;


	return key_W;
}