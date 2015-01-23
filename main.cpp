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
//#include "Player.h"

#define KILL_POINTS 500


// Comments are for those of weak constitutions and simple minds.

int playGame(sf::RenderWindow&, int, Player *, Weapon * );

// Returns pointer to new array of weapons.
// nW is the number of weapons in the new array.
// Weaps is a pointer to element 0 of the current array.
Weapon * populateWeapons(std::string, int *);

Player * populatePlayers(int, int);

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

	Player * players;
	int nPlayers = 4;


	Weapon * weapons;
	int nWeapons;

	weapons = populateWeapons("weapons.csv", &nWeapons);

	//std::cout << nWeapons << '\n';

	players = populatePlayers(nPlayers, nWeapons);


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
            int winner = playGame(window, nPlayers, players, weapons);
            if (winner >= 0) {
                players[winner].wins++;
                std::cout << "Player " << winner << " wins!\n";
            } else std::cout << "TIE!\n";
        } else if (sel == 2) {
            // Configure
			std::cout << "This will remove all saved data! Are you sure you wish to continue? (Yes/No): ";
			std::string YesNo;
			std::cin >> YesNo;
			if (YesNo.compare("Yes")) std::cout << "\nExiting configuration.\n";
			else {
				std::cout << "Enter a number of players:\n";
				std::cin >> nPlayers;

				delete[] players;
				players = populatePlayers(nPlayers, nWeapons);


				// ADD OTHER OPTIONS AND SETTINGS HERE
			}
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


int playGame(sf::RenderWindow & window, int players, Player * scores, Weapon * weapons) {
	srand(time(NULL));

	sandSystem sand(&window);

	std::vector<Projectile> projectiles;
	std::vector<Tank> tanks;

	sand.populate(150.0, 0.45);
	
	int turn = 0;

	// Add tanks!
	for (int i = 0; i < players; i++ ) {
		Tank tank;
		tank = Tank(MAX_HEALTH, &(scores[i])); // Tank health is 1K. Adjust damage accordingly.
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

		int tankRes = 0;

		while (!tankRes && window.isOpen()) {
			tankRes = tanks[turn].controls(deltaTimer.getElapsedTime().asMilliseconds(), weapons);
			deltaTimer.restart();
			timer.restart();
			// Executes until the player shoots.

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

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
					scores[turn].score += KILL_POINTS;
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

Weapon * populateWeapons(std::string filename, int * newNW) {

	std::cout << "Loading weapons...\n";

	//delete *weaps;

	int nW = 0;
	Weapon * weaps = new Weapon[0];

	// TODO: Get weapons from CSV.
	// TODO: Find a way to count weapons.
	std::ifstream fin(filename, std::ios::in);

	if (fin.is_open()) {
		std::string line;
		while (std::getline(fin, line)) {
			if (line[0] != '#') {
				// Create and store as temp new weapon.
				Weapon newWeap = parseWeap(line);

				Weapon*old = weaps;
				weaps = new Weapon[nW+1];
				for (int i = 0; i < nW; i++) {
					weaps[i] = old[i];
				}
				weaps[nW] = newWeap;
				nW++;
				delete[] old;

				// Store old array as temp pointer.
				// Allocate larger array.
			}
		}
		fin.close();
	} else {
		std::cout << "File missing!\n\n";
	}

	for (int i = 0; i < nW; i++) {
		weaps[i].splitResult = &weaps[weaps[i].splitResultInd];
	}

	*newNW = nW;
	std::cout << "Weapons loaded!\n\n";
	return weaps;
}

Player * populatePlayers(int nPlayers, int nWeapons) {
	Player * p = new Player[nPlayers];
	for (int i = 0; i < nPlayers; i++) {
		p[i].score = 0;
		p[i].wins = 0;
		p[i].ammo = new int[nWeapons];
		for (int o = 0; o < nWeapons; o++) p[i].ammo[o] = 100;	// DEBUG
		p[i].nWeapons = nWeapons;
	}
	return p;
}
