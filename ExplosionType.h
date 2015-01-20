#pragma once

enum explosionType {
	circular,		// True inverse square explosion!
	calderic,		// Blows upwards.
	disintegrative,	// Just leaves a hole.
	bunkerbuster	// Boom-pew!
};

enum splitType {
	normal,			// No split
	MIRV,			// Splits once and drops the results.
	napalm,			// Spits a bunch of times before landing.
	flechette		// Splits once, they keep the velocity.
};
