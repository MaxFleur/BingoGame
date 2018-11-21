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

static const bool PREMULT = false;

class MAGIXBSBingoApp : public App {
public:
	void setup() override;
	void draw() override;
	void randomizeBoard();

	cv::Mat drawSquares(cv::Mat input);

	vector<vector<string>> board;
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
	vector<vector<gl::TextureRef>> tBoxes;

	gl::TextureRef mTexture;
};

void MAGIXBSBingoApp::setup()
{
	ci::Surface8u surface(loadImage(loadAsset("BoardGround.jpg")));
	cv::Mat input(toOcv(surface));

	randomizeBoard();
	cv::Mat output = drawSquares(input);

	mTexture = gl::Texture2d::create(fromOcv(input));
	setWindowSize(surface.getWidth(), surface.getWidth());
}

void MAGIXBSBingoApp::randomizeBoard() {

	board.clear();
	vector<string> cloned;
	cloned.resize(bsCases.size());

	copy(begin(bsCases), end(bsCases), begin(cloned));

	for (int x = 0; x <= 4; x++) {
		board.push_back(std::vector<string>());
		for (int y = 0; y <= 4; y++) {
			if (x == 2 && y == 2) {
				board[x].push_back("JOKER!");
			}
			else {
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine randomEveryTime(seed);

				shuffle(begin(cloned), end(cloned), randomEveryTime);
				board[x].push_back(cloned.at(0));

				cloned.erase(cloned.begin());
			}
		}
	}

	cloned.clear();
}

cv::Mat MAGIXBSBingoApp::drawSquares(cv::Mat input) {

	tBoxes.clear();
	tBoxes.reserve(25);

	int height = 50;
	for (int x = 0; x <= 4; x++) {
		int width = 50;
		tBoxes.push_back(std::vector<gl::TextureRef>());
		for (int y = 0; y <= 4; y++) {
			TextBox tbox = TextBox().alignment(TextBox::CENTER).font(Font("Times New Roman", 32)).size(ivec2(158, 158)).text(board[x][y]);
			tbox.setColor(Color(0.0f, 0.0f, 0.0f));
			tbox.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
			gl::TextureRef Texture = gl::Texture2d::create(tbox.render());
			tBoxes[x].push_back(Texture);

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
	gl::clear();
	gl::draw(mTexture);
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

CINDER_APP(MAGIXBSBingoApp, RendererGl, [&](App::Settings *settings) {
	settings->setResizable(false);
})
