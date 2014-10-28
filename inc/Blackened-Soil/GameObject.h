#pragma once

#include "Blackened-Soil/Vector2D.h"
#include <SFML/Graphics.hpp>

class GameObject {
public:
    sf::CircleShape obj;
    sf::Vector2f& pos;// = obj.position;
    Vector2D vel;
    Vector2D acc;

    
};