#include "Tank.h"

void Tank::render(sf::RenderWindow &window){
	sf::CircleShape shape = sf::CircleShape(5);
	shape.setPosition(pos.x, pos.y);
	window.draw(shape);
}

void Tank::update() {}

void Tank::controls() {
	bool key_W = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool key_S = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool key_A = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool key_D = sf::Keyboard::isKeyPressed(sf::Keyboard::D);


}