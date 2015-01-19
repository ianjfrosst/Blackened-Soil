#include "Tank.h"

void Tank::render(sf::RenderWindow &window, bool isMyTurn){
	sf::CircleShape shape = sf::CircleShape(5);
	shape.setPosition(pos.x, SAND_SYSTEM_Y-pos.y-10);
	if (isMyTurn) {
		sf::Text healthPowerAngle;	// TODO
		shape.setFillColor(sf::Color::Red);
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
		 return true;
	 } else {
		 vel.y =0;
		 return false;
	 }
}

void Tank::startTurn() {
	minTurn.restart();
	result = Projectile(pos, Vector2D(0,0));
}

bool Tank::controls(int deltaMillis) {
	bool key_W = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool key_S = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool key_A = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool key_D = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool key_Sp = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	bool key_Sh = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

	bool fired = key_Sp && minTurn.getElapsedTime().asSeconds() > 1;

	if (key_Sh) {
		if (key_W) power += (deltaMillis/100);
		if (key_S) power -= (deltaMillis/100);

		if (key_A) angle += 3.14159*(deltaMillis/5000.0);
		if (key_D) angle -= 3.14159*(deltaMillis/5000.0);
	} else {
		if (key_W) power += (deltaMillis/20);
		if (key_S) power -= (deltaMillis/20);

		if (key_A) angle += 3.14159*(deltaMillis/1000.0);
		if (key_D) angle -= 3.14159*(deltaMillis/1000.0);
	}

	std::cout << "Angle: " << angle << ", Power: " << power << '\n';

	result.vel.SetDM(angle, power);
	result.alive = true;

	

	return fired;
}