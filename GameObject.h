#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>

class GameObject {
public:
    sf::Drawable* obj;
    Vector2D pos;// = obj.position;
    Vector2D vel;
    Vector2D acc;

    GameObject() {}

    void update();

    
};