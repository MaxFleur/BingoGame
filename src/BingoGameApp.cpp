
//Bingo Game, written in C++. Uses LibCinder and OpenCV. Work in progress.
//
// Code provided by MaxFleur
// https://github.com/MaxFleur

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "GameHandler.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

// Bingo Game, this class only stars the applicatipn
class BingoGameApp : public App {
public:
	void setup() override;
	void draw() override;

	void mouseUp(MouseEvent event) override;
	// Game handler to manage the entire board, field entrys and sound
	GameHandlerRef gH;

	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;
};

void BingoGameApp::setup()
{
	// Load Board image 
	auto img = loadImage(loadAsset("BoardGround.jpg"));
	// Generate game handler
	gH = std::make_shared<GameHandler>();
	// Start a new game and determine the file path of the winning sound
	gH->startNewGame();
	gH->setSoundFilePath();

	// Create a texture out of the boardground and set the windows to the actual board size
	mTexture = gl::Texture2d::create(img);
	setWindowSize(900, 1000);
}

// Let the InteractionHandler do the handling stuff
void BingoGameApp::mouseUp(MouseEvent event) {
	gH->handle(event);
}

void BingoGameApp::draw()
{
	// Draw texture of the board and the square meshes
	gl::clear();
	gl::draw(mTexture);
	gH->draw();
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})