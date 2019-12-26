#pragma once

#pragma once
#include "Randomizer.hpp"
#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"
#include "SoundHandler.hpp"


class InputHandler {
public:
	InputHandler(cv::Mat input) : m_input(input) {};
	~InputHandler() {};

	// Randomize strings, then create the board
	void startNewGame() {
		restart = false;
		r->randomize();
		bH->clearStoredTL();
		bH->setupBoard();
		bH->createBoard(m_input, r->getEntrys());
		bLS->resetIsBlack();
	}

	// Handle mouse clicks
	void handle(ci::app::MouseEvent event) {
		// If the restart is true, reset and start a new game
		if (restart == true) {
			startNewGame();
			bH->drawWin = false;
			return;
		}
		// For restarting the game
		else if (event.getX() > 375 && event.getX() < 525 && event.getY() > 925 && event.getY() < 965) {
			startNewGame();
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
				if (bLS->isBlack.at(boxRow).at(boxCol) == false) {
					checked = true;
					textBoxColor = bH->black;
					backgroundColor = bH->white;
				}
				// Otherwise reset
				else {
					checked = false;
					textBoxColor = bH->white;
					backgroundColor = bH->black;
				}
				// Check or uncheck the box
				bLS->isBlack.at(boxRow).at(boxCol) = checked;
				// Set text size depending on length of the string
				bH->textBox.setText(bH->tLs.at(cloneIndex).text);
				bH->textBox.setFont(
					Font(
						"Helvetica",
						bH->tLs.at(cloneIndex).length)
				);

				// Set textbox and background color
				bH->textBox.setColor(textBoxColor);
				bH->textBox.setBackgroundColor(backgroundColor);

				// Render and draw modified texture
				gl::TextureRef Texture = gl::Texture2d::create(bH->textBox.render());
				bH->fieldTextures.at(boxRow).at(boxCol) = Texture;
				gl::draw(bH->fieldTextures[boxRow][boxCol]);
			}
			// If a whole vertical, horizontal or diagonal line is black (and thus the game is won), 
			// the BoardHandler will play the audio file and draw the winning texture
			if (bLS->searchForBlackLine()) {
				bH->drawWin = true;
				sH->playSound();
				restart = true;
			}
		}
	}

	BoardHandlerRef getBoardHandler() { return bH; }
	SoundHandlerRef getSoundHandler() { return sH; }

private:
	RandomizerRef r = std::make_shared<Randomizer>();
	BoardHandlerRef bH = std::make_shared<BoardHandler>();
	BLSRef bLS = std::make_shared<BlackLineSearch>();
	SoundHandlerRef sH = std::make_shared<SoundHandler>();
	cv::Mat m_input;

	bool restart = false;
	bool checked = false;

	Color textBoxColor;
	Color backgroundColor;
};
using InputHandlerRef = std::shared_ptr<InputHandler>;
