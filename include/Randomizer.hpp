
// Randomizes vector entrys, the randomied entrys are used for the bingo fields
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include <random> 
#include <vector>
#include <string>

class Randomizer {
public:
	Randomizer() {};
	~Randomizer() {};

	//Randomize the board
	void randomize() {

		// Shuffle the source vector
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine randomEveryTime(seed);
		shuffle(begin(source), end(source), randomEveryTime);

		// Clear entrys before another run
		entrys.clear();
		// Then copy the first 25 elements of source into entrys
		for (int i = 0; i < 25; i++) {
			if (i == 12) {
				// Joker for entry in the board center
				entrys.push_back("JOKER!");
			}
			else {
				entrys.push_back(source.at(i));
			}
		}
	}

	std::vector<std::string> getEntrys() { return entrys; }

private:

	// Entrys used for board
	std::vector<std::string> entrys;
	// Source vector, contains several strings, just test values for this case
	std::vector<std::string> source = {
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"AA"
	};
};
using RandomizerRef = std::shared_ptr<Randomizer>;