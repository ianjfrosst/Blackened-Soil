#include "GameObject.h"

// Default implementation of update. 
void GameObject::update() {
    vel += acc;
    pos += vel;
}

// Default implementation of draw. Overridable
void GameObject::draw(sf::RenderWindow &window) {
	window.draw(spr);
}
