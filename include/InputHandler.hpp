
// Handles the game instance and user mouse input
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "Randomizer.hpp"
#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"
#include "SoundHandler.hpp"
#include "Randomizer.hpp"

// InputHandler class, handles mouse clicks on the field
class InputHandler {
public:
	InputHandler() {};
	~InputHandler() {};

	// Handle the mouse clicks on the main field
	bool handleMouseInput(ci::app::MouseEvent event, BLSRef bLS, BoardHandlerRef bH, SoundHandlerRef sH) {
		// In any other case modify the box colors
		if (event.isLeft()) {
			float x = event.getX();
			float y = event.getY();
			// If inside the main game field
			if (x < 848 && x > 51 && y < 899 && y > 101
				&& fmod(x - 50, 160) < 158 && fmod(x - 50, 160) > 1
				&& fmod(y - 100, 160) < 159 && fmod(y - 100, 160) > 1) {
				// Generate the matching x and y entrys
				int boxRow = (y - 101) / 160;
				int boxCol = (x - 51) / 160;
				// Index of textlength entrys
				int index = (boxRow * 5) + boxCol;
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
				bH->textBox.setText(bH->tLs.at(index).text);
				bH->textBox.setFont(
					Font(
						"Helvetica",
						bH->tLs.at(index).length)
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
				return true;
			}
		}
		return false;
	}

	// Determine if the mouse click was on the restart button
	bool isClickRestart(ci::app::MouseEvent event) {
		if (event.getX() > 375 && event.getX() < 525 && event.getY() > 925 && event.getY() < 965) {
			return true;
		}
		else return false;
	}

private:
	// Sets bools of isBlack
	bool checked = false;
	// Colors of textBoxes and their background
	Color textBoxColor;
	Color backgroundColor;
};
using InputHandlerRef = std::shared_ptr<InputHandler>;
