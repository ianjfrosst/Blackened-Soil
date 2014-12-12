#pragma once

#include "Vector2D.h"
#include <SFML/Graphics.hpp>

#define SAND_SYSTEM_X 500
#define SAND_SYSTEM_Y 500

struct sandPart {
    Vector2D pos;
    Vector2D vel;
    sf::Color col;

    sandPart() {}
    sandPart(Vector2D p, Vector2D v, sf::Color c) : pos(p), vel(v), col(c) {}
};

struct explosionData {
    int startX;
    int endX;
    Vector2D loc;
    double power;
    double range;
    int occupation;
};
