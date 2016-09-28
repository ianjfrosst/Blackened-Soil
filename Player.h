#include "Weapon.h"


struct Player {
	int score;

	int kills;
	int deaths;
	int suicides;
	
	sf::Color col;
	
	int * ammo;
	int nWeapons;

	int wins;
};
