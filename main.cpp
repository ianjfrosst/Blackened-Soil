#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "Vector2D.h"
#include "Projectile.h"
#include "Sand.h"
#include "ExplosionType.h"
#include "Tank.h"


// Comments are for those of weak constitutions and simple minds.

int playGame(sf::RenderWindow&, int);
int populateWeapons(std::string filename, Weapon ** weaps);

int main() {

	std::cout
	<< "BLACKENED SOIL\n"
	<< "By Charles Holtforster\n"
	<< " & Ian Frosst\n"
	<< "\n"
	<< "For an ICS4UG summative.\n"
	<< "2014-2015\n"
	<< "---------------------------\n\n";

	// Ends execution when set to false.
	bool STOPNOW = true;

	int players = 4;

	std::vector<int> scores;

	Weapon * weapons = new Weapon[1];

	for (int i = 0; i < players; i++) {
		scores.push_back(0);
	}

	populateWeapons("weapons.csv", &weapons);

	while (STOPNOW) {
		std::cout << "Select an option:\n"
                << "1 - Play game!\n"
		        << "2 - Configure\n"
		        << "3 - Controls\n"
                << "4 - Quit\n";

		int sel = 0;
		std::cin >> sel;

		if (sel == 1) {
            sf::ContextSettings settings;
            settings.antialiasingLevel = 1;
            sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "Blackened Soil", sf::Style::Default, settings);
            int winner = playGame(window, players);
            if (winner >= 0) {
                scores[winner]++;
                std::cout << "Player " << winner + 1 << " wins!\n";
            } else std::cout << "TIE!\n";
        } else if (sel == 2) {
            // Configure
        } else if (sel == 3) {
            // Controls
        } else if (sel == 4) {
            STOPNOW = false;
        } else {
            continue;
        }
    }
    return 0;
}


int playGame(sf::RenderWindow & window, int players) {
	srand(time(NULL));

	Weapon defWeap, resWeap;

	defWeap.ExplosionSize = 25;
	defWeap.MaxDamage = 1000;
	defWeap.name = "Really big MOAB";
	defWeap.splitInterval = 3;
	defWeap.splitMaxSpeed = 10;
	defWeap.splitNumber = 50;
	defWeap.splitTime = 5;
	defWeap.splType = splitType::normal;
	defWeap.xplType = explosionType::circular;
	defWeap.splitResult = &resWeap;

	resWeap.ExplosionSize = 50;
	resWeap.MaxDamage = 1000;
	resWeap.name = "Really tiny nuclear device";
	resWeap.splitNumber = 0;
	resWeap.splType = splitType::normal;
	resWeap.xplType = explosionType::bunkerbuster;

	sandSystem sand(&window);

	std::vector<Projectile> projectiles;
	std::vector<Tank> tanks;

	sand.populate(150.0, 0.45);
	
	int turn = 0;

	// Add tanks!
	for (int i = 0; i < players; i++ ) {
		Tank tank;
		tank = Tank(MAX_HEALTH, i, &defWeap); // Tank health is 1K. Adjust damage accordingly.
		Vector2D pos(rand() % SAND_SYSTEM_X,0);
		int y = 0;
		while (sand.staticSand[(int)pos.x][y] != sf::Color::Transparent) y++;
		pos.y = y;
		tank.setPos(pos);
		tanks.push_back(tank);
	}

	sf::Clock timer;

    while (window.isOpen()) {
		sf::Clock deltaTimer;
		tanks[turn].startTurn();

		// --------------------------------------------------
		// GAMEPLAY LOOP
		// --------------------------------------------------

		bool eBrake = true;

		while (!tanks[turn].controls(deltaTimer.getElapsedTime().asMilliseconds()) && window.isOpen() && eBrake) {
			deltaTimer.restart();
			timer.restart();
			// Executes until the player shoots.

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			/*if (lastMouseState != sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
					!lastMouseState &&
					sf::Mouse::getPosition(window).x < SAND_SYSTEM_X &&
					sf::Mouse::getPosition(window).x > 0) {
				sf::Vector2i position = sf::Mouse::getPosition(window);
				sand.detonate(Vector2D(position.x,SAND_SYSTEM_Y-position.y), 100, 50, explosionType::circular);
				eBrake = false;
			}

			lastMouseState = sf::Mouse::isButtonPressed(sf::Mouse::Left);*/

			window.clear();
			
			timer.restart();
			sand.render();
			//std::cout << "Sand render took " << timer.getElapsedTime().asMicroseconds() << " microseconds.\n";

			for (int i = 0; i < tanks.size(); i ++) {
				tanks[i].render(window, i == turn);
			}

			window.display();
			// TODO: Add frame rate lock.
		}
		
		projectiles.push_back(tanks[turn].result);
		projectiles[projectiles.size() - 1].birth.restart();

		turn = (turn+1)%tanks.size();

		//std::cout << "Switching to other mode! Projectiles: " << projectiles.size() << '\n';

		int updateResult = 1;

		// --------------------------------------------------
		// UPDATE LOOP
		// No player input!
		// --------------------------------------------------
		bool tanksUpdated = true;

		while ((sand.update(Vector2D(0,-1)) || projectiles.size() > 0 || tanksUpdated) && window.isOpen()) {
			//std::cout << "UPDATE took " << timer.getElapsedTime().asMilliseconds() << " milliss.\n";
			tanksUpdated = false;
			// This loop will execute until there is no active sand, no projectiles, and no closed window.

			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.clear();
			
			timer.restart();
			sand.render();
			//std::cout << "Sand render took " << timer.getElapsedTime().asMilliseconds() << " millis with active particles.\n";

			for (int i = 0; i < tanks.size(); i++) {
				tanksUpdated = tanksUpdated || tanks[i].update(&sand);
				tanks[i].render(window, false);
				if (tanks[i].health <= 0) {
					tanks.erase(tanks.begin() + i);
					i--;
					if (turn >= i) turn = (turn-1)%tanks.size();  
				}
				if (tanks.size() == 1) return tanks[0].playerNumber;
			}
			
			for (int i = 0; i < projectiles.size(); i++) {
				//std::cout << "Running projectile " << i << ".\n";
				int res = projectiles[i].update(&sand,Vector2D(0,-1));
				projectiles[i].render(window);
				//std::cout << "Projectile code was" << res << ".\n";

				if (res & EXPL_SAD || res & EXPL_SPL) {		// Projeciles is splitting.
					std::vector<Projectile> * newProj = projectiles[i].split();
					for (int o = 0; o < newProj->size(); o++) projectiles.push_back((*newProj)[o]);
					delete newProj;
				}

				if (res & EXPL_DET) {
					//std::cout << "BOOM!\n";
					for (int o = 0; o < tanks.size(); o++) {
						// DEAL DAMAGE!
						tanks[o].takeDamage(projectiles[i].result());
					}
				}

				if (res & EXPL_OOB || res & EXPL_SAD || res & EXPL_DET) { // Projectile is dead for whatever reason.
					projectiles.erase(projectiles.begin() + i);
					i--;
				}
			}
			window.display();
			timer.restart();
			if (tanks.size() == 1) return tanks[0].playerNumber;
			if (tanks.size() == 0) return -1; // We've killed them all....
		}
    }
    return EXIT_SUCCESS;
}

Weapon parseWeap(std::string in) {
	for (int i = 0; i < in.length(); i++) {
		if (in[i] == ' ') in[i] = '-';
		if (in[i] == ',') in[i] = ' ';
	}

	std::stringstream ss;
	ss.clear();
	ss << in;

	std::cout << in << '\n';

	Weapon newWeapon;
	std::string temp;

	ss >> newWeapon.name;		// There's always a leading number in the string....
	ss >> newWeapon.name;
	ss >> temp;
	newWeapon.accessible = temp[0] = 'Y';
	ss >> newWeapon.price;
	ss >> newWeapon.MaxDamage;
	ss >> newWeapon.ExplosionSize;
	ss >> newWeapon.splitInterval;
	ss >> newWeapon.splitMaxSpeed;	 // Shite....
	ss >> newWeapon.splitNumber;
	ss >> newWeapon.splitTime;
	
	ss >> temp;
	newWeapon.splType = splitType::normal;
	if (!temp.compare("normal")) {
		newWeapon.splType = splitType::normal;
	}
	if (!temp.compare("MIRV")) {
		newWeapon.splType = splitType::MIRV;
	}
	if (!temp.compare("napalm")) {
		newWeapon.splType = splitType::napalm;
	}
	if (!temp.compare("flechette")) {
		newWeapon.splType = splitType::flechette;
	}

	ss >> temp;
	newWeapon.xplType = explosionType::circular;
	if (!temp.compare("circular")) {
		newWeapon.xplType = explosionType::circular;
	}
	if (!temp.compare("disintegrative")) {
		newWeapon.xplType = explosionType::disintegrative;
	}
	ss >> newWeapon.splitResultInd;
	return newWeapon;
}

// Return number of weapons.
// Reallocates weaps, leaves no trace of former items.
int populateWeapons(std::string filename, Weapon ** weaps) {

	delete weaps;

	int nW = 0;

	// TODO: Get weapons from CSV.
	// TODO: Find a way to count weapons.
	std::ifstream fin(filename, std::ios::in);

	if (fin.is_open()) {
		std::string line;
		while (std::getline(fin, line)) {
			if (line[0] != '#') {
				// Create and store as temp new weapon.
				Weapon newWeap = parseWeap(line);

				Weapon*old = *weaps;
				*weaps = new Weapon[nW+1];
				for (int i = 0; i < nW; i++) {
					(*weaps)[i] = old[i];
				}
				(*weaps)[nW] = newWeap;
				nW++;

				// Store old array as temp pointer.
				// Allocate larger array.
			}
		}
		fin.close();
	} else {
		std::cout << "File missing!\n\n";
	}

	return nW;
}
