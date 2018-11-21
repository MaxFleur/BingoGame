/*Bingo Game, written in C++. Uses LibCinder and OpenCV. Work in progress.*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include <algorithm>
#include <random> 

#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MAGIXBSBingoApp : public App {
public:

	void setup() override;
	void draw() override;
	// Function to randomize the Bingo Board
	void randomizeBoard();
	// Draw some square meshes and create textboxes für the bingo fields
	cv::Mat drawSquares(cv::Mat input);

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
	// 2D-Vector for the textBoxes used over the bingo board
	vector<vector<gl::TextureRef>> tBoxes;
	// Texture for the ground and the square meshes
	gl::TextureRef mTexture;
};

void MAGIXBSBingoApp::setup()
{
	// Load Board image and create a matrix
	ci::Surface8u surface(loadImage(loadAsset("BoardGround.jpg")));
	cv::Mat input(toOcv(surface));

	// randomizes the strings and draws square meshes over the board
	randomizeBoard();
	cv::Mat output = drawSquares(input);

	//  a texture from all stuff and set the windows to the actual board size
	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getWidth());
}

void MAGIXBSBingoApp::randomizeBoard() {

	// Clear board in case of another started game
	board.clear();
	// Resize and clone vector from board, this vector will be used for randomization
	vector<string> cloned;
	cloned.resize(bsCases.size());

	copy(begin(bsCases), end(bsCases), begin(cloned));

	// Iterate over all fields and give a string to every field
	for (int x = 0; x <= 4; x++) {
		board.push_back(std::vector<string>());
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

cv::Mat MAGIXBSBingoApp::drawSquares(cv::Mat input) {

	// Clear size of textboxes and reserve them 25 fields
	tBoxes.clear();
	tBoxes.reserve(25);

	// Iterates over the board, creating textboxes and square meshes.
	int height = 50;
	for (int x = 0; x <= 4; x++) {
		int width = 50;
		tBoxes.push_back(std::vector<gl::TextureRef>());
		for (int y = 0; y <= 4; y++) {
			// Creates a Textbox, setting color of the text and the background. 
			// Uses the strings from the bsCases-Vector.
			TextBox tbox = TextBox()
				.alignment(TextBox::CENTER)
				.font(Font("Times New Roman", 32))
				.size(ivec2(158, 158))
				.text(board[x][y]);
			tbox.setColor(Color(0.0f, 0.0f, 0.0f));
			tbox.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
			// Create a texture for every testbox and store it for later drawing
			gl::TextureRef Texture = gl::Texture2d::create(tbox.render());
			tBoxes[x].push_back(Texture);

			// Draws the square meshes over the field
			cv::Rect r = cv::Rect(width, height, 160, 160);
			cv::rectangle(input, r, cv::Scalar(80, 80, 80), 2, cv::LINE_8, 0);
			width += 160;
		}
		height += 160;
	}
	return input;
}

void MAGIXBSBingoApp::draw()
{
	// Draw texture of the board and the square meshes
	gl::clear();
	gl::draw(mTexture);
	// Draw textBoxes, iterating over the board. Height and width are set to 51 to create a more stylish look
	int height = 51;
	for (int x = 0; x <= 4; x++) {
		int width = 51;
		for (int y = 0; y <= 4; y++) {
			gl::draw(tBoxes[x][y], vec2(width, height));
			width += 160;
		}
		height += 160;
	}
}

// Set the window so it is not resizable
CINDER_APP(MAGIXBSBingoApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})
