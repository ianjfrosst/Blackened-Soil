#pragma once

#include "Summative/Vector2D.h"
#include <SFML/Graphics.hpp>

class GameObject {
public:
    sf::Rect obj;
    sf::Vector2f& pos = obj.position;
    Vector2D vel;
    Vector2D acc;

    
};