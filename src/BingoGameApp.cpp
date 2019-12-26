
//Bingo Game, written in C++. Uses LibCinder and OpenCV. Work in progress.
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "InputHandler.hpp"

#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// Bingo Game, this class only stars the applicatipn
class BingoGameApp : public App {
public:
	void setup() override;
	void draw() override;

	void mouseUp(MouseEvent event) override;

	InputHandlerRef iH;

	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;
	bool restart;	
};

void BingoGameApp::setup()
{
	// Load Board image and create a matrix
	ci::Surface8u surface(loadImage(loadAsset("BoardGround.jpg")));
	cv::Mat input(toOcv(surface));

	iH = std::make_shared<InputHandler>(input);
	// Start a new game and determine the file path of the winning sound
	iH->startNewGame();
	iH->getSoundHandler()->handleSoundFilePath();

	// Create a texture from all stuff and set the windows to the actual board size
	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getHeight());
}

// Let the InteractionHandler do the handling stuff
void BingoGameApp::mouseUp(MouseEvent event) {
	iH->handle(event);
}

void BingoGameApp::draw()
{
	// Draw texture of the board and the square meshes
	gl::clear();
	gl::draw(mTexture);
	iH->getBoardHandler()->draw();
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})