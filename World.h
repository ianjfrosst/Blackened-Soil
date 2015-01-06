#pragma once

#include "Sand.h"
#include "Tank.h"
#include "GameObject.h"
#include "Projectile.h"

#include <SFML/Window.hpp>

#include <vector>


class World {
	sandSystem map;
	std::vector<Tank> tanks;
	std::vector<Projectile> projectiles;

public :
	void render(sf::RenderWindow);
	void update();
};