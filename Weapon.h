#pragma once

//#include <map>
#include "ExplosionType.h"
#include "Vector2D.h"

struct Weapon {
	std::string name;
	int ExplosionSize;
	int	MaxDamage;		// Later resolved via invSqr
	
	explosionType xplType;
	splitType	  splType;

	float		splitTime;
	float		splitInterval;
	int			splitNumber;
	Weapon*		splitResult;
	Vector2D	splitMaxSpeed;
};

//std::map<std::string, Weapon> weapons;

