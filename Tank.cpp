#include "Tank.h"

#define TANK_RADIUS 5

sf::Vector2f Tank::transformPoint(Vector2D in) {
	return sf::Vector2f(pos.x + in.x,SAND_SYSTEM_Y-pos.y+in.y);
}

void Tank::render(sf::RenderWindow &window, bool isMyTurn){
	sf::CircleShape shape = sf::CircleShape(TANK_RADIUS);
	shape.setPosition(pos.x, SAND_SYSTEM_Y-pos.y-10);
	if (isMyTurn) {
		// Aim
		Vector2D relPos;
		relPos.SetDM(angle, power);
		sf::Vertex aimLine[] = {
			sf::Vertex(sf::Vector2f(pos.x+5,(SAND_SYSTEM_Y-pos.y)-5)),
			sf::Vertex(sf::Vector2f((pos+relPos).x+5,SAND_SYSTEM_Y-(pos+relPos).y-5))
		};
		aimLine[1].color = sf::Color::Red;
		window.draw(aimLine, 2, sf::Lines);

		shape.setFillColor(sf::Color::Red);
	} else {
		// Health
		Vector2D relPos;
		relPos.SetDM(angle, power);
		sf::Vertex aimLine[] = {
			sf::Vertex(sf::Vector2f(pos.x+15,(SAND_SYSTEM_Y-pos.y)-15)),
			sf::Vertex(sf::Vector2f(pos.x-5,(SAND_SYSTEM_Y-pos.y)-15)),
			sf::Vertex(sf::Vector2f(pos.x-5,(SAND_SYSTEM_Y-pos.y)-15)),
			sf::Vertex(sf::Vector2f(pos.x-5+((health/MAX_HEALTH)*20.0),(SAND_SYSTEM_Y-pos.y)-15))
		};
		aimLine[0].color = sf::Color::Red;
		aimLine[1].color = sf::Color::Red;
		aimLine[2].color = sf::Color::Green;
		aimLine[3].color = sf::Color::Green;
		window.draw(aimLine, 4, sf::Lines);
	}
	window.draw(shape);
}

void Tank::setPos(Vector2D p) {
	pos = p;
}

bool Tank::update(sandSystem * sand) {
	pos += vel;
	
	if (sand->staticSand[(int)pos.x][(int)pos.y] == sf::Color::Transparent) {
		vel.y -= 1;
	} else {
		vel.y = 0;
	}
	if (pos.y < 0) {
		pos.y = 0;
		vel.y = 0;
		if (sand->staticSand[(int)pos.x][0] == sf::Color::Transparent) health = 0;
	}
	return vel.y != 0;
}

void Tank::startTurn(Weapon * weapons) {
	minTurn.restart();
	result = Projectile(pos, Vector2D(0,0));
	std::cout << "Player " << playerNumber+1 << "'s turn!\n";
	std::cout << "\nCurrently weilding " << weapons[weaponSelection].name << ". " << player->ammo[weaponSelection] << " ammo available.\n";
}

int Tank::controls(int deltaMillis, Weapon * weapons) {
	bool key_Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool key_Dn = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool key_L = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool key_R = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	bool key_Sp = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	bool key_Sh = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

	bool key_PgUp = sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket);

	if (key_PgUp && !lastChangeKey) {
		weaponSelection = (weaponSelection+1) % player->nWeapons;
		while (!weapons[weaponSelection].accessible) weaponSelection = (weaponSelection+1) % player->nWeapons;
		std::cout << "Selected " << weapons[weaponSelection].name << ". " << player->ammo[weaponSelection] << " ammo available.\n";
	}
    lastChangeKey = key_PgUp;

	bool fired = key_Sp && minTurn.getElapsedTime().asSeconds() > 1 && player->ammo[weaponSelection] > 0;

	if (key_Sh) {
		if (key_Up) power += (deltaMillis/100.0);
		if (key_Dn) power -= (deltaMillis/100.0);

		if (key_L) angle += 3.14159*(deltaMillis/5000.0);
		if (key_R) angle -= 3.14159*(deltaMillis/5000.0);
	} else {
		if (key_Up) power += (deltaMillis/10.0);
		if (key_Dn) power -= (deltaMillis/10.0);

		if (key_L) angle += 3.14159*(deltaMillis/1000.0);
		if (key_R) angle -= 3.14159*(deltaMillis/1000.0);
	}
    int maxPower = 35;
    if (power > maxPower) power = maxPower;
    if (power < 0) power = 0;


	result.pos.SetXY(pos.x+5,pos.y+5);
	result.vel.SetDM(angle, power);
	result.weap = &(weapons[weaponSelection]);
	result.framesTill = result.weap->splitTime;
    
	if (fired) player->ammo[weaponSelection] --;

	return fired;
}

bool Tank::takeDamage(explosion expl) {
	float dist = (expl.pos - (pos+Vector2D(TANK_RADIUS,TANK_RADIUS))).GetMag();
	if (dist < expl.size) {
		dist = (dist > TANK_RADIUS ? dist : 0);
		float dmg = ((expl.size-dist)/expl.size) * expl.maxDMG;
		std::cout << "Shot with " << expl.weap->name << " at " << dist << '/' << expl.weap->ExplosionSize
			<< " pixels, dealing " << dmg << " damage.\n";
		health -= dmg;
	}
	return health > 0;
}

bool Tank::checkProjectile(Projectile * incoming) {
    //return false; //. DEBUG!
	//if (incoming.vel.x > TANK_RADIUS || incoming.vel.y > TANK_RADIUS)

	float dist = (pos+Vector2D(TANK_RADIUS,TANK_RADIUS)).GetSegmentDist(incoming->pos, incoming->pos+incoming->vel);

	std::cout << "Projectile is " << dist << " px away.\n";
	if (dist < TANK_RADIUS) {
		std::cout << "Hit with projectile!\n";
		incoming->pos = pos+Vector2D(TANK_RADIUS,TANK_RADIUS);
		//health -= incoming->weap->MaxDamage;
		return true;
	}
	return false;
}
