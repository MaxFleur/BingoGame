
// Handles the interaction of mouse clicks on the board
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "GameHandler.hpp"

class InteractionHandler : public BlackLineSearch {
public:
	InteractionHandler() {};
	~InteractionHandler() {};

	void handle(ci::app::MouseEvent event, std::shared_ptr<GameHandler> gameHandler) {

		if (restart == true) {
			restart = false;
			gameHandler->startNewGame();
			gameHandler->getBoardHandler()->drawWin = false;
			return;
		}
		else if (event.getX() > 375 && event.getX() < 525 && event.getY() > 925 && event.getY() < 965) {
			gameHandler->startNewGame();
			return;
		}
		else if (event.isLeft()) {
			float x = event.getX();
			float y = event.getY();

			if (x < 848 && x > 51 && y < 899 && y > 101
				&& fmod(x - 50, 160) < 158 && fmod(x - 50, 160) > 1
				&& fmod(y - 100, 160) < 159 && fmod(y - 100, 160) > 1) {

				int boxRow = (y - 101) / 160;
				int boxCol = (x - 51) / 160;

				int cloneIndex = (boxRow * 5) + boxCol;

				gameHandler->getBoardHandler()->isBlack.at(boxRow).at(boxCol) = true;

				gameHandler->getBoardHandler()->textBox.setText(gameHandler->getRandomizer()->getEntrys().at(cloneIndex));
				gameHandler->getBoardHandler()->textBox.setColor(Color(0.96f, 0.96f, 0.96f));
				gameHandler->getBoardHandler()->textBox.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));

				gl::TextureRef Texture = gl::Texture2d::create(gameHandler->getBoardHandler()->textBox.render());
				gameHandler->getBoardHandler()->fieldTextures.at(boxRow).at(boxCol) = Texture;
				gl::draw(gameHandler->getBoardHandler()->fieldTextures[boxRow][boxCol]);
			}

			if (BlackLineSearch::searchForBlackLine(gameHandler->getBoardHandler()->isBlack)) {
				gameHandler->getBoardHandler()->drawWin = true;
				gameHandler->getBoardHandler()->getVoice()->start();
				restart = true;
			}
		}
	}
	bool restart = false;
};
using InteractionHandlerRef = std::shared_ptr<InteractionHandler>;