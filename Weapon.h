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

	int			splitTime;
	int			splitInterval;

	int			splitNumber;
	Weapon*		splitResult;
	Vector2D	splitMaxSpeed;

	Weapon () {
		name = "M240 \"Boring weapon\"";
		ExplosionSize = 50;
		MaxDamage = 500;

		xplType = explosionType::circular;
		splType = splitType::normal;

		splitTime = 0;
		splitInterval = 0;
		splitNumber = 0;
		splitResult = this;
		splitMaxSpeed = Vector2D(10,10);
	}
};

//std::map<std::string, Weapon> weapons;

