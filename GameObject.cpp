#include "GameObject.h"

// Default implementation of update. 
void GameObject::update() {
	pos += vel;
}

void GameObject::render(sf::RenderWindow &window) {
	//window.draw(spr);
}
