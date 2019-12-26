
// Creates the bingo board (textures, game text)
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"

using namespace ci;

class BoardHandler {
public:
	BoardHandler() {};
	~BoardHandler() {};
	// Store the text entrys and their determining the length
	struct storeTL {
		std::string text;
		int length;
	};

	// Setup all textures and textBox needed for board initialization
	void setupBoard() {
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
	}

	// Creates board
	void createBoard(std::vector<std::string> entrys) {
		// Clear and reserve textures
		fieldTextures.clear();
		fieldTextures.reserve(25);

		// Center all text
		textBox.setAlignment(ci::TextBox::CENTER);
		textBox.setSize(ci::ivec2(157, 157));

		int height = 100;
		int entryIndex = 0;

		// Iterates over the board, creating textboxes and square meshes.
		for (int x = 0; x <= 4; x++) {
			int width = 50;

			fieldTextures.push_back(std::vector<ci::gl::TextureRef>());

			for (int y = 0; y <= 4; y++) {
				int textSize;
				// Set text size depending on input length
				if (entrys.at(entryIndex).length() > 30) {
					textSize = 27;
				}
				else {
					textSize = 32;
				}
				// Set font and text based on randomization
				textBox.setFont(Font("Helvetica", textSize));
				textBox.setText(entrys.at(entryIndex));

				storeTL m_storeTL{ entrys.at(entryIndex), textSize };
				tLs.push_back(m_storeTL);

				// Black background and white text for centered field
				if (x == 2 && y == 2) {
					textBox.setColor(black);
					textBox.setBackgroundColor(white);
				}
				// White background and black text for all other fields
				else {
					textBox.setColor(white);
					textBox.setBackgroundColor(black);
				}
				// Create a texture for every testbox and store it for later drawing
				gl::TextureRef Texture = gl::Texture2d::create(textBox.render());
				fieldTextures[x].push_back(Texture);

				width += 160;
				entryIndex++;
			}
			height += 160;
		}
		// Points for the polylines
		ci::vec2 horizontalLeft = ci::vec2(48, 100);
		ci::vec2 horizontalRight = ci::vec2(852, 100);
		ci::vec2 verticalTop = ci::vec2(50, 100);
		ci::vec2 verticalBottom = ci::vec2(50, 900);
		// Clear polylines before another generation
		lines.clear();
		// Draw six horizontal and six vertical lines
		for (int i = 0; i <= 5; i++) {
			// Horizontal line, push back the horizontal points
			ci::PolyLine2f horizontalLine;
			horizontalLine.push_back(horizontalLeft);
			horizontalLine.push_back(horizontalRight);
			lines.push_back(horizontalLine);
			// Same for vertical points and line
			ci::PolyLine2f verticalLine;
			verticalLine.push_back(verticalTop);
			verticalLine.push_back(verticalBottom);
			lines.push_back(verticalLine);
			// Move the points coordinates for the next two polylines
			horizontalLeft.y += 160;
			horizontalRight.y += 160;
			verticalTop.x += 160;
			verticalBottom.x += 160;
		}
	}

	// Draw board with field textures and polylines
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
		// Draw restart and header texture
		gl::draw(restartTexture, vec2(375, 925));
		gl::draw(headerTexture, vec2(100, 20));
		// Set linewidth and greyvalue
		gl::lineWidth(4);
		gl::color(Color(0.31f, 0.31f, 0.31f));
		// Then draw all polylines and reset color
		for (int i = 0; i < lines.size(); i++) {
			gl::draw(lines.at(i));
		}
		gl::color(Color::white());

		// Only draw winning texture if condition is true
		if (drawWin == true) {
			gl::draw(winningTexture, vec2(200, 130));
		}
	}

	gl::TextureRef getWinTexture() { return winningTexture; }
	// Textures of the fields
	std::vector<std::vector<ci::gl::TextureRef>> fieldTextures;
	// texBox used for the textures
	TextBox textBox;
	// Store texts and determining lengths
	std::vector<storeTL> tLs;
	// Draw things when game is won
	bool drawWin;

	Color white = Color(0.03f, 0.03f, 0.03f);
	Color black = Color(0.96f, 0.96f, 0.96f);

private:
	// Texture displayed when game is won
	gl::TextureRef winningTexture;
	// Texture displayed for game restart
	gl::TextureRef restartTexture;
	// Header
	gl::TextureRef headerTexture;
	std::vector<ci::PolyLine2f> lines;
};
using BoardHandlerRef = std::shared_ptr<BoardHandler>;