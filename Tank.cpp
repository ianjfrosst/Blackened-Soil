#include "Tank.h"

sf::Vector2f Tank::transformPoint(Vector2D in) {
	return sf::Vector2f(pos.x + in.x,SAND_SYSTEM_Y-pos.y+in.y);
}

void Tank::render(sf::RenderWindow &window, bool isMyTurn){
	sf::CircleShape shape = sf::CircleShape(5);
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

void Tank::startTurn() {
	minTurn.restart();
	result = Projectile(pos, Vector2D(0,0));
}

int Tank::controls(int deltaMillis) {
	bool key_W = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool key_S = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool key_A = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool key_D = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	bool key_Sp = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	bool key_Sh = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);


	bool fired = key_Sp && minTurn.getElapsedTime().asSeconds() > 1;

	if (key_Sh) {
		if (key_W) power += (deltaMillis/100.0);
		if (key_S) power -= (deltaMillis/100.0);

		if (key_A) angle += 3.14159*(deltaMillis/5000.0);
		if (key_D) angle -= 3.14159*(deltaMillis/5000.0);
	} else {
		if (key_W) power += (deltaMillis/20.0);
		if (key_S) power -= (deltaMillis/20.0);

		if (key_A) angle += 3.14159*(deltaMillis/1000.0);
		if (key_D) angle -= 3.14159*(deltaMillis/1000.0);
	}
    int maxPower = 35;
    if (power > maxPower) power = maxPower;
    if (power < 0) power = 0;


	result.pos.SetXY(pos.x+5,pos.y+5);
	result.vel.SetDM(angle, power);
	result.weap = weap;
	result.framesTill = weap->splitTime;
    
	return fired;
}

bool Tank::takeDamage(explosion expl) {
	std::cout << "Checking bullet hit...\n";
	std::cout << "Am " << (expl.pos - pos).GetMag() << " pixels away.\n";
	if ((expl.pos - pos).GetMag() < expl.size) {
		float dmg = ((expl.size-(expl.pos - pos).GetMag())/expl.size) * expl.maxDMG;
		std::cout << "Shot with " << expl.weap->name << ".\n";
		health -= dmg;
	}
	return health > 0;
}