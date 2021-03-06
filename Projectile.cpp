#include "Projectile.h"

void Projectile::render(sf::RenderWindow &window) {
	if (trace.size() > 1) for (int i = 1; i < trace.size(); i++) {
		sf::Vertex line[] = {
			sf::Vertex(trace[i]),
			sf::Vertex(trace[i-1])
		};
		if (i+1 == trace.size()) line[0].color = sf::Color::Red;
		window.draw(line, 2, sf::Lines);
	}
}
	
explosion Projectile::result() {
	return explosion(pos, weap->ExplosionSize, weap->MaxDamage, weap);
}

std::vector<Projectile>* Projectile::split() {
	std::vector<Projectile> * res = new std::vector<Projectile>;
	for (int i = 0; i < weap->splitNumber; i++) {
		Vector2D randspeed;
		randspeed.SetDM(rand(),weap->splitMaxSpeed);
		if (weap->splType == splitType::MIRV)
			res->push_back(Projectile(pos, vel + randspeed, weap->splitResult));
		if (weap->splType == splitType::napalm)
			res->push_back(Projectile(pos, Vector2D(vel.x*0.05,vel.y), weap->splitResult));
		if (weap->splType == splitType::flechette)
			res->push_back(Projectile(pos, (vel*0.5) + randspeed, weap->splitResult));
	}
	return res;
}

int Projectile::update(sandSystem * world, Vector2D influence) {
	trace.push_back(sf::Vector2f(pos.x, SAND_SYSTEM_Y-pos.y));
	
	int res = 0;

	pos += vel;
	vel += influence;
	//std::cout << "->" << framesTill << "\n";

	if (weap->splType != splitType::normal) {
		if (!splitting && framesTill > 0) {
			framesTill--;
			if (framesTill == 0) {
				splitting = true;
				framesTill = weap->splitInterval;
				
			}
			//std::cout << "Starting to split!\n";
		}
		if (splitting && framesTill > 0) {
			framesTill--;
			if (framesTill == 0) {
				res += (weap->splType==splitType::MIRV ? EXPL_SAD : EXPL_SPL);
				framesTill = weap->splitInterval;
			}
			//std::cout << "Splitting!\n";
		}
	}

	if (pos.y < SAND_SYSTEM_Y && pos.y > 0 && pos.x > 0 && pos.x < SAND_SYSTEM_X) {
		if (world->staticSand[(int)pos.x][(int)pos.y] != sf::Color::Transparent) {
			world->detonate(pos, weap->ExplosionSize, weap->ExplosionSize, weap->xplType);
			res += EXPL_DET;
			//return EXPL_DET;
		}
	}

	// The round is out of bounds
	if (pos.x < 0 || pos.x > SAND_SYSTEM_X || pos.y < 0) return EXPL_OOB;//res = res | EXPL_OOB;


	return res;
}

