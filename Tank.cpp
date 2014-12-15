#include "Tank.h"

void Tank::render(sf::RenderWindow &window){
	sf::CircleShape img(10);
	img.setFillColor(sf::Color::Red);
	img.setPosition(pos.x,pos.y);
}

void Tank::update() {
	//sf::Keyboard::isKeyPressed(sf::Keyboard::W);

}