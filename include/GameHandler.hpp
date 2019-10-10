#pragma once
#include "Randomizer.hpp"
#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"
#include "CinderOpenCV.h"

class GameHandler {
public: 
	GameHandler();
	~GameHandler();

	void startNewGame(cv::Mat input) {
		r->randomize();
		bH->setup();
		bH->createBoard(input, r->getEntrys());
	}

private:
	RandomizerRef r = std::make_shared<Randomizer>();
	BoardHandlerRef bH = std::make_shared<BoardHandler>();
	BLSRef bLS = std::make_shared<BlackLineSearch>();
};
using GameHandlerRef = std::shared_ptr<GameHandler>;