
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

	void startNewGame(cv::Mat input);

	void mouseUp(MouseEvent event) override;

	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;
	cv::Mat inputGameStart;
	bool restart;

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
	inputGameStart = input;
	// randomizes the strings and draws square meshes over the board
	startNewGame(inputGameStart);

	// Create a texture from all stuff and set the windows to the actual board size
	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getHeight());
}

void BingoGameApp::mouseUp(MouseEvent event) {

	if (event.isLeft()) {
		if (restart == true) {
			startNewGame(inputGameStart);
			restart = false;
		}
		else {
		}
	}
}

void BingoGameApp::startNewGame(cv::Mat input) {
	r->randomize();
	bH->setup();
	bH->createBoard(input, r->getEntrys());
}

void BingoGameApp::draw()
{
	// Draw texture of the board and the square meshes
	gl::clear();
	gl::draw(mTexture);

	bH->draw();
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})
