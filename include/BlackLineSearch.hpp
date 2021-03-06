
// This class checks if a vertical, diagonal or horizontal line is ompleteley black 
// (and thus the Bingo Game is won)
//
// Thanks to Jonathan Janetzki for providing the optimized code!
// https://github.com/janetzki
// Further providing by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include <assert.h>
#include <vector>

class BlackLineSearch {
public:
	BlackLineSearch() {};
	~BlackLineSearch() {};

	bool searchForBlackLine() {
		// works for any n x m matrix with n > 0 and m > 0
		const size_t width = isBlack.size();
		assert(width > 0);
		const size_t height = isBlack[0].size();
		bool hasBlackLine;
		// search for vertical lines
		for (int x = 0; x < width; x++) {
			hasBlackLine = true;
			for (int y = 0; y < height; y++) {
				hasBlackLine &= isBlack[x][y];
			}
			if (hasBlackLine) {
				return true;
			}
		}
		// search for horizontal lines
		for (int y = 0; y < height; y++) {
			hasBlackLine = true;
			for (int x = 0; x < width; x++) {
				hasBlackLine &= isBlack[x][y];
			}
			if (hasBlackLine) {
				return true;
			}
		}
		// search for diagonal lines
		if (width != height) {
			return false;
		}
		hasBlackLine = true;
		for (int x = 0; x < width; x++) {
			hasBlackLine &= isBlack[x][x];
		}
		if (hasBlackLine) {
			return true;
		}
		hasBlackLine = true;
		for (int x = 0; x < width; x++) {
			hasBlackLine &= isBlack[x][width - x - 1];
		}
		return hasBlackLine;
	}

	// Reset in case of a new game
	void resetIsBlack() {
		// True in the middle of the game
		isBlack = {
			{false, false, false, false, false},
			{ false, false, false, false, false },
			{ false, false, true, false, false },
			{ false, false, false, false, false },
			{ false, false, false, false, false }
		};
	}

	std::vector<std::vector<bool>> isBlack;
};
using BLSRef = std::shared_ptr<BlackLineSearch>;