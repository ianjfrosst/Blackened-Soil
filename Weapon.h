#pragma once

#include "ExplosionType.h"
#include "Vector2D.h"

class Weapon {
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
public :

};