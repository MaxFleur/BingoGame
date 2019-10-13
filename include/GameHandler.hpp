
// Creates new game instances when called
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "Randomizer.hpp"
#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"

// Instances new game at application start or if game is restartet
class GameHandler {
public: 
	GameHandler(cv::Mat input) : m_input(input) {};
	~GameHandler() {};

	// Randomize strings, then create the board
	void startNewGame() {
		r->randomize();
		bH->setupBoard();
		bH->createBoard(m_input, r->getEntrys());
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