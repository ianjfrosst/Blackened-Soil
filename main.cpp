#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <fstream>

#include "Vector2D.h"
#include "Projectile.h"
#include "Tank.h"
//#include "Player.h"

#define KILL_POINTS 500
#define SUICIDE_DEDUCTION 1000


// Comments are for those of weak constitutions and simple minds.

int playGame(sf::RenderWindow&, int, Player *, Weapon * );

// Returns pointer to new array of weapons.
// nW is the number of weapons in the new array.
// Weaps is a pointer to element 0 of the current array.
Weapon * populateWeapons(std::string, int *);

Player * populatePlayers(int, int);

int main() {

	// TEST================================

	Vector2D a(0,0);
	Vector2D b(0,10);
	Vector2D c(20,20);

	// Nba * (c-a).Nba should be within the bounding box of b <-> a + 1 in either direction.

	Vector2D Nba = (b-a).Normal();

	double DP = abs((c-a).DotProduct(Nba));

	std::cout << "Dot product of "  << (c-a) << " and " << Nba << ": " << DP << " = " << DP/sqrt(2) << "\n";
	std::cout << "Proximity of " << c << " to " << a << " and " << b << ": " << c.GetSegmentDist(a,b) << "\n";
	
	// This bit works. Now for actually proximity to segment.

	//std::cout << c.GetSegmentDist(a,b) << '\n';

	// TEST================================

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

	players = populatePlayers(nPlayers, nWeapons);

	while (STOPNOW) {
		std::cout << "Select an option:\n"
                << "1 - Play game!\n"
		        << "2 - Configure\n"
		        << "3 - Controls\n"
				<< "4 - Scoreboard\n"
                << "5 - Quit\n";

		int sel = 0;
		std::cin >> sel;

		if (sel == 1) {
            sf::ContextSettings settings;
            settings.antialiasingLevel = 1;
            sf::RenderWindow window(sf::VideoMode(SAND_SYSTEM_X, SAND_SYSTEM_Y, 32), "Blackened Soil", sf::Style::Default, settings);
            int winner = playGame(window, nPlayers, players, weapons);
            if (winner >= 0) {
                players[winner].wins++;
                std::cout << "Player " << winner+1 << " wins!\n";
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
			std::cout
				<< "Controls:\n\n"
				<< "Aim/power:\t\tArrow keys\n"
				<< "Fire:\t\t\tSpace\n"
				<< "Change weapon:\t\t[\n"
				<< "Precision aim:\t\tLeft shift\n\n";

		} else if (sel == 4) {
            std::cout
				<< "Scores:\n"
				<< "#\t|Score:\t|Kills:\t|Deaths:|Self-kills:\t|Wins:\n"
				<< "____________________________________________________________\n";

			for (int i = 0; i < nPlayers; i++) std::cout << i+1 << "\t|" << players[i].score << "\t|"<< players[i].kills <<"\t|"<< players[i].kills <<"\t|" <<players[i].suicides
				<<"\t\t|" <<players[i].wins<<'\n';
        } else if (sel == 5) {
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

	sand.populate(200.0, 0.45);
	
	int turn = 0;

	// Add tanks!
	for (int i = 0; i < players; i++ ) {
		Tank tank;
		tank = Tank(MAX_HEALTH, &(scores[i]), i); // Tank health is 1K. Adjust damage accordingly.
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
		tanks[turn].startTurn(weapons);

		// --------------------------------------------------
		// GAMEPLAY LOOP
		// --------------------------------------------------

		int tankRes = 0;

		int curPlayer = tanks[turn].playerNumber;

		bool windowHasFocus = true;

		while (!tankRes && window.isOpen()) {
			if (windowHasFocus) tankRes = tanks[turn].controls(deltaTimer.getElapsedTime().asMilliseconds(), weapons);
			else tankRes = 0;
            deltaTimer.restart();
			timer.restart();
			// Executes until the player shoots.

			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::LostFocus)
					windowHasFocus = false;
				if (event.type == sf::Event::GainedFocus)
					windowHasFocus = true;
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

			//std::thread renderThread(&sandSystem::render, sand);

			//renderThread.join();

			sand.render();
			//std::cout << "Sand render took " << timer.getElapsedTime().asMilliseconds() << " millis with active particles.\n";

			
			for (int i = 0; i < projectiles.size(); i++) {
				//std::cout << "Running projectile " << i << ".\n";
				int res = projectiles[i].update(&sand,Vector2D(0,-1));
				projectiles[i].render(window);
				//std::cout << "Projectile code was" << res << ".\n";

				bool hitTank = false;
				for (int o = 0; o < tanks.size(); o++) {
					if (curPlayer != tanks[o].playerNumber) hitTank = hitTank || tanks[o].checkProjectile(&(projectiles[i]));
				}

				if (res & EXPL_SAD || res & EXPL_SPL) {		// Projeciles is splitting.
					std::vector<Projectile> * newProj = projectiles[i].split();
					for (int o = 0; o < newProj->size(); o++) projectiles.push_back((*newProj)[o]);
					delete newProj;
				}

				if (res & EXPL_DET || hitTank) {
					//std::cout << "BOOM!\n";
					for (int o = 0; o < tanks.size(); o++) {
						// DEAL DAMAGE!
						tanks[o].takeDamage(projectiles[i].result());
					}
				}

				if (res & EXPL_OOB || res & EXPL_SAD || res & EXPL_DET || hitTank) { // Projectile is dead for whatever reason.
					projectiles.erase(projectiles.begin() + i);
					i--;
				}
			}

			for (int i = 0; i < tanks.size(); i++) {
				tanksUpdated = tanksUpdated || tanks[i].update(&sand);
				tanks[i].render(window, false);
				if (tanks[i].health <= 0) {
					std::cout << "Player " << curPlayer+1 << " killed player " << tanks[i].playerNumber+1 << '\n';
					scores[curPlayer].score += (curPlayer==tanks[i].playerNumber?-SUICIDE_DEDUCTION:KILL_POINTS);
					if (curPlayer==tanks[i].playerNumber) scores[curPlayer].suicides++;
					else scores[curPlayer].kills++;
					scores[tanks[i].playerNumber].deaths++;


					tanks.erase(tanks.begin() + i);
					i--;

					if (turn >= i) turn = (turn-1)%tanks.size();
				}
				if (tanks.size() == 1) return tanks[0].playerNumber;
			}


			window.display();
			timer.restart();
			if (tanks.size() == 1) return tanks[0].playerNumber;
			if (tanks.size() == 0) return -1; // We've killed them all....
		}



    }
    return -1;
}

Weapon parseWeap(std::string in) {
	for (int i = 0; i < in.length(); i++) {
		if (in[i] == ' ') in[i] = '-';
		if (in[i] == ',') in[i] = ' ';
	}

	std::stringstream ss;
	ss.clear();
	ss << in;

	//std::cout << in << '\n';

	Weapon newWeapon;
	std::string temp;

	ss >> newWeapon.name;		// There's always a leading number in the string....
	ss >> newWeapon.name;
	ss >> temp;
	newWeapon.accessible = temp[0] == 'Y';
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
	if (!temp.compare("calderic")) {
		newWeapon.xplType = explosionType::calderic;
	}
	ss >> newWeapon.splitResultInd;
	return newWeapon;
}

Weapon * populateWeapons(std::string filename, int * newNW) {

	std::cout << "Loading weapons...\n";

	//delete *weaps;

	int nW = 0;
	Weapon * weaps = new Weapon[0];

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

		p[i].kills = 0;
		p[i].suicides = 0;
		p[i].deaths = 0;

		p[i].ammo = new int[nWeapons];
		for (int o = 0; o < nWeapons; o++) p[i].ammo[o] = 5;	// DEBUG
		p[i].nWeapons = nWeapons;
	}
	return p;
}
