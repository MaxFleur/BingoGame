// Handles the game instance - starts a new game, arranges mouse inputs and handles the spound and game drawing
//
// Code provided by MaxFleur
// https://github.com/MaxFleur

#pragma once
#include "Randomizer.hpp"
#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"
#include "SoundHandler.hpp"
#include "InputHandler.hpp"

// InputHandler class, start a new game and handle a mouse clicks
class GameHandler {
public:
	GameHandler() {};
	~GameHandler() {};

	// Start a new game
	void startNewGame() {
		restart = false;
		// First, randomize the field entrys
		r->randomize();
		// Then clear stored text and textlengths in case of another game instance
		bH->tLs.clear();
		// Setup and generate new board
		bH->setupBoard();
		bH->createBoard(r->getEntrys());
		// Reset bools in case of another game instance
		bLS->resetIsBlack();
	}

	// Handle the mouse input from the main game
	// Determine if the game shall be restarted ir not
	void handle(ci::app::MouseEvent event) {
		// If the restart is true, reset and start a new game
		if (restart == true) {
			startNewGame();
			bH->drawWin = false;
			return;
		}
		// Else, if the restart button was clicked, restart
		if (iH->isClickRestart(event)) {
			startNewGame();
			return;
		}
		// else do a normal mouse input
		if (iH->handleMouseInput(event, bLS, bH, sH)) {
			restart = true;
		}
	}

	void setSoundFilePath() {
		sH->handleSoundFilePath();
	}

	void draw() {
		bH->draw();
	}

private:
	RandomizerRef r = std::make_shared<Randomizer>();
	BoardHandlerRef bH = std::make_shared<BoardHandler>();
	BLSRef bLS = std::make_shared<BlackLineSearch>();
	SoundHandlerRef sH = std::make_shared<SoundHandler>();
	InputHandlerRef iH = std::make_shared<InputHandler>();
	// used to determine game restarts
	bool restart = false;
};
using GameHandlerRef = std::shared_ptr<GameHandler>;
