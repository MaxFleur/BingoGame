
// Creates the bingo board (textures, game text)
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "BlackLineSearch.hpp"
#include "BoardHandler.hpp"
#include "Randomizer.hpp"

class MouseClickHandler : public BoardHandler, public BlackLineSearch, public Randomizer {
public:
	MouseClickHandler() {};
	~MouseClickHandler() {};

	void handleMouseEvent(ci::app::MouseEvent event) {
		float x = event.getX();
		float y = event.getY();

		if (x < 848 && x > 51 && y < 899 && y > 101
			&& fmod(x - 50, 160) < 158 && fmod(x - 50, 160) > 1
			&& fmod(y - 100, 160) < 159 && fmod(y - 100, 160) > 1) {

			int boxRow = (y - 101) / 160;
			int boxCol = (x - 51) / 160;

			int cloneIndex = (boxRow * 5) + boxCol;

			BoardHandler::isBlack.at(boxRow).at(boxCol) = true;

			BoardHandler::textBox.setText(r->getEntrys().at(cloneIndex));
			BoardHandler::textBox.setColor(Color(0.96f, 0.96f, 0.96f));
			BoardHandler::textBox.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));

			gl::TextureRef Texture = gl::Texture2d::create(BoardHandler::textBox.render());
			BoardHandler::fieldTextures.at(boxRow).at(boxCol) = Texture;
			gl::draw(BoardHandler::fieldTextures[boxRow][boxCol]);

			if (BlackLineSearch::searchForBlackLine(BoardHandler::isBlack)) {
				BoardHandler::getVoice()->start();
				//restart = true;
			}
		}
		if (x > 375 && x < 525 && y > 925 && y < 965) {
			//startNewGame();
		}
	}
private:

};