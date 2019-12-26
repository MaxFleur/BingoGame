
// Handles the interaction of mouse clicks on the board
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "GameHandler.hpp"

// Handles interactions via mouse clicks
class InteractionHandler : public BlackLineSearch {
public:
	InteractionHandler() {};
	~InteractionHandler() {};

	// Handle mouse clicks
	void handle(ci::app::MouseEvent event, std::shared_ptr<GameHandler> gameHandler) {
		// If the restart is true, reset and start a new game
		if (restart == true) {
			restart = false;
			gameHandler->startNewGame();
			gameHandler->getBoardHandler()->drawWin = false;
			return;
		}
		// For restarting the game
		else if (event.getX() > 375 && event.getX() < 525 && event.getY() > 925 && event.getY() < 965) {
			gameHandler->startNewGame();
			return;
		}
		// In any other case create a black box with white text
		else if (event.isLeft()) {
			float x = event.getX();
			float y = event.getY();

			if (x < 848 && x > 51 && y < 899 && y > 101
				&& fmod(x - 50, 160) < 158 && fmod(x - 50, 160) > 1
				&& fmod(y - 100, 160) < 159 && fmod(y - 100, 160) > 1) {

				int boxRow = (y - 101) / 160;
				int boxCol = (x - 51) / 160;

				int cloneIndex = (boxRow * 5) + boxCol;
				// Check if the box has been clicked on already
				// Black board and white text if yes
				if (gameHandler->getBlackLineSearch()->isBlack.at(boxRow).at(boxCol) == false) {
					checked = true;
					textBoxColor = gameHandler->getBoardHandler()->black;
					backgroundColor = gameHandler->getBoardHandler()->white;
				}
				// Otherwise reset
				else {
					checked = false;
					textBoxColor = gameHandler->getBoardHandler()->white;
					backgroundColor = gameHandler->getBoardHandler()->black;
				}
				// Check or uncheck the box
				gameHandler->getBlackLineSearch()->isBlack.at(boxRow).at(boxCol) = checked;
				// Set text size depending on length of the string
				gameHandler->getBoardHandler()->textBox.setText(gameHandler->getBoardHandler()->tLs.at(cloneIndex).text);
				gameHandler->getBoardHandler()->textBox.setFont(
					Font(
						"Helvetica", 
						gameHandler->getBoardHandler()->tLs.at(cloneIndex).length)
					);

				// Set textbox and background color
				gameHandler->getBoardHandler()->textBox.setColor(textBoxColor);
				gameHandler->getBoardHandler()->textBox.setBackgroundColor(backgroundColor);

				// Render and draw modified texture
				gl::TextureRef Texture = gl::Texture2d::create(gameHandler->getBoardHandler()->textBox.render());
				gameHandler->getBoardHandler()->fieldTextures.at(boxRow).at(boxCol) = Texture;
				gl::draw(gameHandler->getBoardHandler()->fieldTextures[boxRow][boxCol]);
			}
			// If a whole vertical, horizontal or diagonal line is black (and thus the game is won), 
			// the BoardHandler will play the audio file and draw the winning texture
			if (gameHandler->getBlackLineSearch()->searchForBlackLine()) {
				gameHandler->getBoardHandler()->drawWin = true;
				gameHandler->getSoundHandler()->playSound();
				restart = true;
			}
		}
	}

private:
	bool restart = false;
	bool checked = false;

	Color textBoxColor;
	Color backgroundColor;
};
using InteractionHandlerRef = std::shared_ptr<InteractionHandler>;