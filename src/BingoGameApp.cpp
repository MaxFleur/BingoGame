
//Bingo Game, written in C++. Uses LibCinder and OpenCV. Work in progress.
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include <algorithm>

#include "Randomizer.hpp"
#include "BoardHandler.hpp"
#include "BlackLineSearch.hpp"

#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BingoGameApp : public App, public BoardHandler {
public:
	void setup() override;
	void draw() override;

	void mouseUp(MouseEvent event) override;

	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;

	RandomizerRef r;
	BoardHandlerRef bH;
	BLSRef bLS;
};

void BingoGameApp::setup()
{
	r		= std::make_shared<Randomizer>();
	bH		= std::make_shared<BoardHandler>();
	bLS		= std::make_shared<BlackLineSearch>();

	// Load Board image and create a matrix
	ci::Surface8u surface(loadImage(loadAsset("BoardGround.jpg")));
	cv::Mat input(toOcv(surface));

	// randomizes the strings and draws square meshes over the board
	r->randomize();
	bH->setup();
	bH->createBoard(input, r->getEntrys());

	// Create a texture from all stuff and set the windows to the actual board size
	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getHeight());
}

void BingoGameApp::mouseUp(MouseEvent event) {

	if (event.isLeft()) {
		if (bH->getRestart() == true) {
			setup();
			bH->setRestart(false);
		}
		else {
			float x = event.getX();
			float y = event.getY();

			if (x < 848 && x > 51 && y < 899 && y > 101
				&& fmod(x - 50, 160) < 158 && fmod(x - 50, 160) > 1
				&& fmod(y - 100, 160) < 159 && fmod(y - 100, 160) > 1) {

				int boxRow = (y - 101) / 160;
				int boxCol = (x - 51) / 160;

				int cloneIndex = (boxRow * 5) + boxCol;

				bH->isBlack.at(boxRow).at(boxCol) = true;

				bH->textBox.setText(r->getEntrys().at(cloneIndex));
				bH->textBox.setColor(Color(0.96f, 0.96f, 0.96f));
				bH->textBox.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));

				gl::TextureRef Texture = gl::Texture2d::create(bH->textBox.render());
				bH->fieldTextures.at(boxRow).at(boxCol) = Texture;
				gl::draw(bH->fieldTextures[boxRow][boxCol]);
				
				if (bLS->searchForBlackLine(bH->isBlack)) {
					bH->getVoice()->start();
					bH->setRestart(true);
				}
			}
			if (x > 375 && x < 525 && y > 925 && y < 965) {
				setup();
			}
		}
	}
}

void BingoGameApp::draw()
{
	// Draw texture of the board and the square meshes
	gl::clear();
	gl::draw(mTexture);

	bH->draw();

	// Draw textBoxes, iterating over the board. Height and width are set to 51 to create a more stylish look
	if (bLS->searchForBlackLine(bH->isBlack)) {
		gl::draw(bH->getWinTexture(), vec2(200, 130));
	}
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})
