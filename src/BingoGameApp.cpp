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

	// 2D-Vector, represents the board
	vector<vector<string>> board;
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
		"Y"
	};

	vector<vector<TextBox>> textBoxes; 

	vector<vector<bool>> isBlack;

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
	string header = "DAS EPISCHSTE BINGO-SPIEL ALLER ZEITEN!";
	TextBox tboxHeader = TextBox().alignment(TextBox::CENTER).font(Font("Helvetica", 40)).size(ivec2(700, 40)).text(header);
	tboxHeader.setColor(Color(0.03f, 0.03f, 0.03f));
	tboxHeader.setBackgroundColor(Color(0.85f, 0.55f, 0.32f));
	headerTexture = gl::Texture2d::create(tboxHeader.render());

	string won = "YEAH, DU HAST GEWONNEN! ABER LEIDER GIBTS KEINEN PREIS. SPIEL DOCH EINFACH NOCHMAL!";
	TextBox tboxWon = TextBox().alignment(TextBox::CENTER).font(Font("Helvetica", 32)).size(ivec2(360, 120)).text(won);
	tboxWon.setColor(Color(1.0f, 0.839f, 0.0f));
	tboxWon.setBackgroundColor(Color(0.289f, 0.125f, 0.23f));
	winningTexture = gl::Texture2d::create(tboxWon.render());

	string restart = "Neustarten";
	TextBox tboxRestart = TextBox().alignment(TextBox::CENTER).font(Font("Helvetica", 32)).size(ivec2(150, 40)).text(restart);
	tboxRestart.setColor(Color(0.03f, 0.03f, 0.03f));
	tboxRestart.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
	restartTexture = gl::Texture2d::create(tboxRestart.render());

	audio::SourceFileRef sourceFile = audio::load(app::loadAsset("win.mp3"));
	mVoice = audio::Voice::create(sourceFile);
	mVoice->setVolume(15);

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
	board.clear();
	// Resize and clone vector from board, this vector will be used for randomization
	vector<string> cloned;
	cloned.resize(bsCases.size());

	copy(begin(bsCases), end(bsCases), begin(cloned));

	// Iterate over all fields and give a string to every field
	for (int x = 0; x <= 4; x++) {
		board.push_back(vector<string>());
		for (int y = 0; y <= 4; y++) {
			// Joker in the middle of the field
			if (x == 2 && y == 2) {
				board[x].push_back("JOKER!");
			}
			else {
				// Set a random variable and shuffle the cloned vector with every iteration
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine randomEveryTime(seed);

				shuffle(begin(cloned), end(cloned), randomEveryTime);
				// Push back every first element of the vector
				board[x].push_back(cloned.at(0));
				// Erase used element. This prevents the use of the same element multiple times
				cloned.erase(cloned.begin());
			}
		}
	}
	// Clear cloned vector (really necessary?)
	cloned.clear();
}

cv::Mat BingoGameApp::drawSquares(cv::Mat input) {

	// Clear size of textboxes and reserve them 25 fields
	texturesFromTextBoxes.clear();
	texturesFromTextBoxes.reserve(25);

	textBoxes.clear();
	textBoxes.reserve(25);

	isBlack.clear();

	// Iterates over the board, creating textboxes and square meshes.
	int height = 100;
	for (int x = 0; x <= 4; x++) {
		int width = 50;
		texturesFromTextBoxes.push_back(vector<gl::TextureRef>());
		textBoxes.push_back(vector<TextBox>());
		isBlack.push_back(vector<bool>());

		for (int y = 0; y <= 4; y++) {

			// Creates a Textbox, setting color of the text and the background. 
			// Uses the strings from the bsCases-Vector.
			TextBox tbox = TextBox().alignment(TextBox::CENTER).font(Font("Helvetica", 32)).size(ivec2(158, 158)).text(board[x][y]);
			if(x == 2 && y == 2) {
				isBlack[x].push_back(true);
				tbox.setColor(Color(0.96f, 0.96f, 0.96f));
				tbox.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));
			}
			else {
				isBlack[x].push_back(false);
				tbox.setColor(Color(0.0f, 0.0f, 0.0f));
				tbox.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
			}
			textBoxes[x].push_back(tbox);
			// Create a texture for every testbox and store it for later drawing
			gl::TextureRef Texture = gl::Texture2d::create(tbox.render());
			texturesFromTextBoxes[x].push_back(Texture);

			// Draws the square meshes over the field
			cv::Rect r = cv::Rect(width, height, 160, 160);
			cv::rectangle(input, r, cv::Scalar(80, 80, 80), 2, cv::LINE_8, 0);
			width += 160;
		}
		height += 160;
	}
	cv::Rect r = cv::Rect(375, 925, 150, 40);
	cv::rectangle(input, r, cv::Scalar(80, 80, 80), 3, cv::LINE_8, 0);
	cv::Rect h = cv::Rect(100, 20, 700, 40);
	cv::rectangle(input, h, cv::Scalar(80, 80, 80), 3, cv::LINE_8, 0);
	return input;
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

		|| isBlack.at(0).at(0) == true && isBlack.at(1).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(3).at(3) == true && isBlack.at(4).at(4) == true
		|| isBlack.at(4).at(0) == true && isBlack.at(3).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(1).at(3) == true && isBlack.at(0).at(4) == true) {

		gl::draw(winningTexture, vec2(270, 130));
	}

}


void BingoGameApp::mouseUp(MouseEvent event) {

	if (event.isLeft()) {
		int x = event.getX();
		int y = event.getY();

		if (x < 849 && x > 50 && y < 900 && y > 100) {

			int boxRow = (y - 101) / 160;
			int boxCol = (x - 51) / 160;

			isBlack.at(boxRow).at(boxCol) = true;

			textBoxes[boxRow][boxCol].setColor(Color(0.96f, 0.96f, 0.96f));
			textBoxes[boxRow][boxCol].setBackgroundColor(Color(0.03f, 0.03f, 0.03f));

			gl::TextureRef Texture = gl::Texture2d::create(textBoxes[boxRow][boxCol].render());
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

				|| isBlack.at(0).at(0) == true && isBlack.at(1).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(3).at(3) == true && isBlack.at(4).at(4) == true
				|| isBlack.at(4).at(0) == true && isBlack.at(3).at(1) == true && isBlack.at(2).at(2) == true && isBlack.at(1).at(3) == true && isBlack.at(0).at(4) == true) {

				mVoice->start();
			}
		}
		if (x > 375 && x < 525 && y > 925 && y < 965) {
			setup();
		}
	}
}

// Set the window so it is not resizable
CINDER_APP(BingoGameApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})
