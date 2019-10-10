#pragma once
#include "Randomizer.hpp"
#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"
#include "CinderOpenCV.h"

class GameHandler {
public: 
	GameHandler(cv::Mat input) : m_input(input) {};
	~GameHandler() {};

	void startNewGame() {
		r->randomize();
		bH->setup();
		bH->createBoard(m_input, r->getEntrys());
	}

	void drawBoard() {
		bH->draw();
	}

	BoardHandlerRef getBoardHandler() { return bH; }
	RandomizerRef getRandomizer() { return r; }

private:
	RandomizerRef r = std::make_shared<Randomizer>();
	BoardHandlerRef bH = std::make_shared<BoardHandler>();
	BLSRef bLS = std::make_shared<BlackLineSearch>();
	cv::Mat m_input;
};
using GameHandlerRef = std::shared_ptr<GameHandler>;