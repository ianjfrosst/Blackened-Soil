#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>

class GameObject {
public:
    sf::Sprite spr;
    Vector2D pos;
    Vector2D vel;
    Vector2D acc;

    GameObject() {}

    virtual void update();
	virtual void draw(sf::RenderWindow &window);
};
