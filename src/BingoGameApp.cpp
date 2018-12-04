/*Bingo Game, written in C++. Uses LibCinder and OpenCV. Work in progress.*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/audio/audio.h"
#include <algorithm>
#include <random> 

#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BingoGameApp : public App {
public:
	void setup() override;
	void draw() override;
	// Function to randomize the Bingo Board
	void randomizeBoard();
	// Draw some square meshes and create textboxes für the bingo fields
	cv::Mat drawSquares(cv::Mat input);

	void mouseUp(MouseEvent event) override;

	vector<string> clone;
	// Several strings, for usage within the bingo board. These strings are stored and used in a cloned vector.
	vector<string> bsCases = {
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"AA"
	};

	TextBox bfT;

	vector<vector<bool>> isBlack;
	bool restart;

	// 2D-Vector for the textBoxes used over the bingo board
	vector<vector<gl::TextureRef>> texturesFromTextBoxes;
	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;
	gl::TextureRef winningTexture;
	gl::TextureRef restartTexture;
	gl::TextureRef headerTexture;

	audio::VoiceRef mVoice;
};

void BingoGameApp::setup()
{
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
	randomizeBoard();
	cv::Mat output = drawSquares(input);

	// Create a texture from all stuff and set the windows to the actual board size
	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getHeight());

}

void BingoGameApp::randomizeBoard() {

	// Clear board in case of another started game
	clone.clear();
	// Resize and clone vector from board, this vector will be used for randomization
	clone.resize(bsCases.size());
	copy(begin(bsCases), end(bsCases), begin(clone));

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine randomEveryTime(seed);

	shuffle(begin(clone), end(clone), randomEveryTime);

	clone.resize(25);
	clone.at(12) = "JOKER!";
	int a = 0;
}

cv::Mat BingoGameApp::drawSquares(cv::Mat input) {

	// Clear size of textboxes and reserve them 25 fields
	texturesFromTextBoxes.clear();
	texturesFromTextBoxes.reserve(25);
	isBlack.clear();

	bfT.setAlignment(TextBox::CENTER);
	bfT.setSize(ivec2(157, 157));

	// Iterates over the board, creating textboxes and square meshes.
	int height = 100;
	int cloneIndex = 0;
	for (int x = 0; x <= 4; x++) {
		int width = 50;
		texturesFromTextBoxes.push_back(vector<gl::TextureRef>());
		isBlack.push_back(vector<bool>());

		for (int y = 0; y <= 4; y++) {

			int textSize;
			if (clone.at(cloneIndex).length() > 30) {
				textSize = 27;
			}
			else {
				textSize = 32;
			}

			bfT.setFont(Font("Helvetica", textSize));
			bfT.setText(clone.at(cloneIndex));
			if(x == 2 && y == 2) {
				isBlack[x].push_back(true);
				bfT.setColor(Color(0.96f, 0.96f, 0.96f));
				bfT.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));
			}
			else {
				isBlack[x].push_back(false);
				bfT.setColor(Color(0.0f, 0.0f, 0.0f));
				bfT.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
			}
			// Create a texture for every testbox and store it for later drawing
			gl::TextureRef Texture = gl::Texture2d::create(bfT.render());
			texturesFromTextBoxes[x].push_back(Texture);

			// Draws the square meshes over the field
			cv::Rect r = cv::Rect(width, height, 160, 160);
			cv::rectangle(input, r, cv::Scalar(80, 80, 80), 2, cv::LINE_8, 0);

			width += 160;
			cloneIndex++;
		}
		height += 160;
	}
	cv::Rect r = cv::Rect(375, 925, 150, 40);
	cv::rectangle(input, r, cv::Scalar(80, 80, 80), 3, cv::LINE_8, 0);
	cv::Rect h = cv::Rect(100, 20, 700, 40);
	cv::rectangle(input, h, cv::Scalar(80, 80, 80), 3, cv::LINE_8, 0);
	return input;
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

				isBlack.at(boxRow).at(boxCol) = true;

				bfT.setText(clone.at(cloneIndex));
				bfT.setColor(Color(0.96f, 0.96f, 0.96f));
				bfT.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));

				gl::TextureRef Texture = gl::Texture2d::create(bfT.render());
				texturesFromTextBoxes.at(boxRow).at(boxCol) = Texture;
				gl::draw(texturesFromTextBoxes[boxRow][boxCol]);

				if (isBlack.at(0).at(0) == true && isBlack.at(0).at(1) == true && isBlack.at(0).at(2) == true && isBlack.at(0).at(3) == true && isBlack.at(0).at(4) == true
					|| isBlack.at(1).at(0) == true && isBlack.at(1).at(1) == true && isBlack.at(1).at(2) == true && isBlack.at(1).at(3) == true && isBlack.at(1).at(4) == true
					|| isBlack.at(2).at(0) == true && isBlack.at(2).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(2).at(3) == true && isBlack.at(2).at(4) == true
					|| isBlack.at(3).at(0) == true && isBlack.at(3).at(1) == true && isBlack.at(3).at(2) == true && isBlack.at(3).at(3) == true && isBlack.at(3).at(4) == true
					|| isBlack.at(4).at(0) == true && isBlack.at(4).at(1) == true && isBlack.at(4).at(2) == true && isBlack.at(4).at(3) == true && isBlack.at(4).at(4) == true

					|| isBlack.at(0).at(0) == true && isBlack.at(1).at(0) == true && isBlack.at(2).at(0) == true && isBlack.at(3).at(0) == true && isBlack.at(4).at(0) == true
					|| isBlack.at(0).at(1) == true && isBlack.at(1).at(1) == true && isBlack.at(2).at(1) == true && isBlack.at(3).at(1) == true && isBlack.at(4).at(1) == true
					|| isBlack.at(0).at(2) == true && isBlack.at(1).at(2) == true && isBlack.at(2).at(2) == true && isBlack.at(3).at(2) == true && isBlack.at(4).at(2) == true
					|| isBlack.at(0).at(3) == true && isBlack.at(1).at(3) == true && isBlack.at(2).at(3) == true && isBlack.at(3).at(3) == true && isBlack.at(4).at(3) == true
					|| isBlack.at(0).at(4) == true && isBlack.at(1).at(4) == true && isBlack.at(2).at(4) == true && isBlack.at(3).at(4) == true && isBlack.at(4).at(4) == true

					|| isBlack.at(0).at(0) == true && isBlack.at(1).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(3).at(3) == true && isBlack.at(4).at(4) == true
					|| isBlack.at(4).at(0) == true && isBlack.at(3).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(1).at(3) == true && isBlack.at(0).at(4) == true) {

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
			gl::draw(texturesFromTextBoxes[x][y], vec2(width, height));
			width += 160;
		}
		height += 160;
	}

	gl::draw(restartTexture, vec2(375, 925));
	gl::draw(headerTexture, vec2(100, 20));

	if (isBlack.at(0).at(0) == true && isBlack.at(0).at(1) == true && isBlack.at(0).at(2) == true && isBlack.at(0).at(3) == true && isBlack.at(0).at(4) == true
		|| isBlack.at(1).at(0) == true && isBlack.at(1).at(1) == true && isBlack.at(1).at(2) == true && isBlack.at(1).at(3) == true && isBlack.at(1).at(4) == true
		|| isBlack.at(2).at(0) == true && isBlack.at(2).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(2).at(3) == true && isBlack.at(2).at(4) == true
		|| isBlack.at(3).at(0) == true && isBlack.at(3).at(1) == true && isBlack.at(3).at(2) == true && isBlack.at(3).at(3) == true && isBlack.at(3).at(4) == true
		|| isBlack.at(4).at(0) == true && isBlack.at(4).at(1) == true && isBlack.at(4).at(2) == true && isBlack.at(4).at(3) == true && isBlack.at(4).at(4) == true

		|| isBlack.at(0).at(0) == true && isBlack.at(1).at(0) == true && isBlack.at(2).at(0) == true && isBlack.at(3).at(0) == true && isBlack.at(4).at(0) == true
		|| isBlack.at(0).at(1) == true && isBlack.at(1).at(1) == true && isBlack.at(2).at(1) == true && isBlack.at(3).at(1) == true && isBlack.at(4).at(1) == true
		|| isBlack.at(0).at(2) == true && isBlack.at(1).at(2) == true && isBlack.at(2).at(2) == true && isBlack.at(3).at(2) == true && isBlack.at(4).at(2) == true
		|| isBlack.at(0).at(3) == true && isBlack.at(1).at(3) == true && isBlack.at(2).at(3) == true && isBlack.at(3).at(3) == true && isBlack.at(4).at(3) == true
		|| isBlack.at(0).at(4) == true && isBlack.at(1).at(4) == true && isBlack.at(2).at(4) == true && isBlack.at(3).at(4) == true && isBlack.at(4).at(4) == true

		|| isBlack.at(0).at(0) == true && isBlack.at(1).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(3).at(3) == true && isBlack.at(4).at(4) == true
		|| isBlack.at(4).at(0) == true && isBlack.at(3).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(1).at(3) == true && isBlack.at(0).at(4) == true) {

		gl::draw(winningTexture, vec2(200, 130));
	}
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})