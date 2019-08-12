
//Bingo Game, written in C++. Uses LibCinder and OpenCV. Work in progress.
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/audio/audio.h"
#include <algorithm>

#include "Randomizer.hpp"
#include "BoardSetup.hpp"
#include "BlackLineSearch.hpp"

#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BingoGameApp : public App {
public:
	void setup() override;
	void draw() override;

	void mouseUp(MouseEvent event) override;

	bool restart;

	// 2D-Vector for the textBoxes used over the bingo board
	vector<vector<gl::TextureRef>> texturesFromTextBoxes;
	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;
	gl::TextureRef winningTexture;
	gl::TextureRef restartTexture;
	gl::TextureRef headerTexture;

	audio::VoiceRef mVoice;

	RandomizerRef r;
	BoardCreatorRef bS;
	BLSRef bLS;
};

void BingoGameApp::setup()
{
	r		= std::make_shared<Randomizer>();
	bS		= std::make_shared<BoardSetup>();
	bLS		= std::make_shared<BlackLineSearch>();

	string header = "DAS EPISCHSTE BULLSHIT-BINGO DER WELT!";
	TextBox tBoxSetup = TextBox().alignment(TextBox::CENTER).font(Font("Helvetica", 40)).size(ivec2(700, 40)).text(header);
	tBoxSetup.setColor(Color(0.03f, 0.03f, 0.03f));
	tBoxSetup.setBackgroundColor(Color(0.85f, 0.55f, 0.32f));
	headerTexture = gl::Texture2d::create(tBoxSetup.render());

	tBoxSetup.setText("YEAH, DU HAST GEWONNEN! \nABER LEIDER GIBTS KEINEN PREIS. \nSPIEL DOCH EINFACH NOCHMAL! \nKLICKE, UM DAS SPIEL NEUZUSTARTEN.");
	tBoxSetup.setFont(Font("Helvetica", 32));
	tBoxSetup.setSize(vec2(500, 120));
	tBoxSetup.setColor(Color(1.0f, 0.839f, 0.0f));
	tBoxSetup.setBackgroundColor(Color(0.289f, 0.125f, 0.23f));
	winningTexture = gl::Texture2d::create(tBoxSetup.render());

	tBoxSetup.setText("Neustarten");
	tBoxSetup.setSize(vec2(150, 40));
	tBoxSetup.setColor(Color(0.03f, 0.03f, 0.03f));
	tBoxSetup.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
	restartTexture = gl::Texture2d::create(tBoxSetup.render());

	audio::SourceFileRef sourceFile = audio::load(app::loadAsset("win.mp3"));
	mVoice = audio::Voice::create(sourceFile);
	mVoice->setVolume(8);

	// Load Board image and create a matrix
	ci::Surface8u surface(loadImage(loadAsset("BoardGround.jpg")));
	cv::Mat input(toOcv(surface));

	// randomizes the strings and draws square meshes over the board
	r->randomize();
	cv::Mat output = bS->createBoard(input, r->getEntrys());

	// Create a texture from all stuff and set the windows to the actual board size
	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getHeight());
}

void BingoGameApp::mouseUp(MouseEvent event) {

	if (event.isLeft()) {
		if (restart == true) {
			setup();
			restart = false;
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

				bS->isBlack.at(boxRow).at(boxCol) = true;

				bS->textBox.setText(r->getEntrys().at(cloneIndex));
				bS->textBox.setColor(Color(0.96f, 0.96f, 0.96f));
				bS->textBox.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));

				gl::TextureRef Texture = gl::Texture2d::create(bS->textBox.render());
				bS->fieldTextures.at(boxRow).at(boxCol) = Texture;
				gl::draw(bS->fieldTextures[boxRow][boxCol]);
				
				if (bLS->searchForBlackLine(bS->isBlack)) {
					mVoice->start();
					restart = true;
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
	// Draw textBoxes, iterating over the board. Height and width are set to 51 to create a more stylish look
	int height = 101;
	for (int x = 0; x <= 4; x++) {
		int width = 51;
		for (int y = 0; y <= 4; y++) {
			gl::draw(bS->fieldTextures[x][y], vec2(width, height));
			width += 160;
		}
		height += 160;
	}

	gl::draw(restartTexture, vec2(375, 925));
	gl::draw(headerTexture, vec2(100, 20));

	if (bLS->searchForBlackLine(bS->isBlack)) {
		gl::draw(winningTexture, vec2(200, 130));
	}
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})
