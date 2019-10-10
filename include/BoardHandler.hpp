
// Creates the bingo board (textures, game text)
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCv.h"
#include "cinder/audio/audio.h"

using namespace ci;

class BoardHandler {
public:
	BoardHandler() {};
	~BoardHandler() {};

	void setup() {
		// Set header text, color and background
		std::string header = "DAS EPISCHSTE BULLSHIT-BINGO DER WELT!";
		TextBox tBoxSetup = TextBox().alignment(TextBox::CENTER).font(Font("Helvetica", 40)).size(ivec2(700, 40)).text(header);
		tBoxSetup.setColor(Color(0.03f, 0.03f, 0.03f));
		tBoxSetup.setBackgroundColor(Color(0.85f, 0.55f, 0.32f));
		headerTexture = gl::Texture2d::create(tBoxSetup.render());
		// Same for winning text
		tBoxSetup.setText("YEAH, DU HAST GEWONNEN! \nABER LEIDER GIBTS KEINEN PREIS. \nSPIEL DOCH EINFACH NOCHMAL! \nKLICKE, UM DAS SPIEL NEUZUSTARTEN.");
		tBoxSetup.setFont(Font("Helvetica", 32));
		tBoxSetup.setSize(vec2(500, 120));
		tBoxSetup.setColor(Color(1.0f, 0.839f, 0.0f));
		tBoxSetup.setBackgroundColor(Color(0.289f, 0.125f, 0.23f));
		winningTexture = gl::Texture2d::create(tBoxSetup.render());
		// Same for restart
		tBoxSetup.setText("Neustarten");
		tBoxSetup.setSize(vec2(150, 40));
		tBoxSetup.setColor(Color(0.03f, 0.03f, 0.03f));
		tBoxSetup.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
		restartTexture = gl::Texture2d::create(tBoxSetup.render());
		// Handle audio file
		ci::audio::SourceFileRef sourceFile = audio::load(app::loadAsset("win.mp3"));
		mVoice = audio::Voice::create(sourceFile);
		mVoice->setVolume(8);
	}

	void createBoard(cv::Mat input, std::vector<std::string> entrys) {
		// Clear and reserve
		fieldTextures.clear();
		fieldTextures.reserve(25);
		isBlack.clear();
		// Center all text
		textBox.setAlignment(ci::TextBox::CENTER);
		textBox.setSize(ci::ivec2(157, 157));

		// Iterates over the board, creating textboxes and square meshes.
		int height = 100;
		int entryIndex = 0;

		for (int x = 0; x <= 4; x++) {
			int width = 50;
			fieldTextures.push_back(std::vector<ci::gl::TextureRef>());
			isBlack.push_back(std::vector<bool>());

			for (int y = 0; y <= 4; y++) {
				int textSize;
				// Set text size depending on input length
				if (entrys.at(entryIndex).length() > 30) {
					textSize = 27;
				}
				else {
					textSize = 32;
				}

				textBox.setFont(Font("Helvetica", textSize));
				textBox.setText(entrys.at(entryIndex));
				// Black background and white text for centered field
				if (x == 2 && y == 2) {
					isBlack[x].push_back(true);
					textBox.setColor(Color(0.96f, 0.96f, 0.96f));
					textBox.setBackgroundColor(Color(0.03f, 0.03f, 0.03f));
				}
				// White background and black text for all other fields
				else {
					isBlack[x].push_back(false);
					textBox.setColor(Color(0.0f, 0.0f, 0.0f));
					textBox.setBackgroundColor(Color(0.96f, 0.96f, 0.96f));
				}
				// Create a texture for every testbox and store it for later drawing
				gl::TextureRef Texture = gl::Texture2d::create(textBox.render());
				fieldTextures[x].push_back(Texture);

				// Draws the square meshes over the field
				cv::Rect r = cv::Rect(width, height, 160, 160);
				cv::rectangle(input, r, cv::Scalar(80, 80, 80), 2, cv::LINE_8, 0);

				width += 160;
				entryIndex++;
			}
			height += 160;
		}
		// Rectangles for game restart and headline
		cv::Rect r = cv::Rect(375, 925, 150, 40);
		cv::rectangle(input, r, cv::Scalar(80, 80, 80), 3, cv::LINE_8, 0);
		cv::Rect h = cv::Rect(100, 20, 700, 40);
		cv::rectangle(input, h, cv::Scalar(80, 80, 80), 3, cv::LINE_8, 0);
	}

	void draw() {
		int height = 101;
		for (int x = 0; x <= 4; x++) {
			int width = 51;
			for (int y = 0; y <= 4; y++) {
				gl::draw(fieldTextures[x][y], vec2(width, height));
				width += 160;
			}
			height += 160;
		}

		gl::draw(restartTexture, vec2(375, 925));
		gl::draw(headerTexture, vec2(100, 20));

	}

	bool getRestart() { return restart; }
	gl::TextureRef getWinTexture() { return winningTexture; }

	// Winning sound
	audio::VoiceRef getVoice() { return mVoice; }

	void setRestart(bool newRestart) { restart = newRestart; }

	std::vector<std::vector<ci::gl::TextureRef>> fieldTextures;
	TextBox textBox;
	// Stores fields clicked on
	std::vector<std::vector<bool>> isBlack;

private:

	bool restart;
	gl::TextureRef winningTexture;
	gl::TextureRef restartTexture;
	gl::TextureRef headerTexture;
	audio::VoiceRef mVoice;
};
using BoardHandlerRef = std::shared_ptr<BoardHandler>;