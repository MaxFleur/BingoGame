#pragma once

#include <random> 
#include <vector>
#include <string>

class Randomizer {
public:
	Randomizer() {};
	~Randomizer() {};

	void randomize() {

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine randomEveryTime(seed);
		shuffle(begin(source), end(source), randomEveryTime);

		entrys.clear();
		for (int i = 0; i < 25; i++) {
			if (i == 12) {
				entrys.push_back("JOKER!");
			}
			else {
				entrys.push_back(source.at(i));
			}
		}
	}

	std::vector<std::string> getEntrys() { return entrys; }

private:
	std::vector<std::string> entrys;
	// Several strings, for usage within the bingo board. These strings are stored and used in a cloned vector.
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