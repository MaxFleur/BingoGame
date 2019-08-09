
// Creates the bingo board (textures, game text)
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCv.h"

class BoardCreator {
public:
	BoardCreator() {};
	~BoardCreator() {};

	cv::Mat createBoard(cv::Mat input, std::vector<std::string> entrys) {
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

				textBox.setFont(ci::Font("Helvetica", textSize));
				textBox.setText(entrys.at(entryIndex));
				// Black background and white text for centered field
				if (x == 2 && y == 2) {
					isBlack[x].push_back(true);
					textBox.setColor(ci::Color(0.96f, 0.96f, 0.96f));
					textBox.setBackgroundColor(ci::Color(0.03f, 0.03f, 0.03f));
				}
				// White background and black text for all other fields
				else {
					isBlack[x].push_back(false);
					textBox.setColor(ci::Color(0.0f, 0.0f, 0.0f));
					textBox.setBackgroundColor(ci::Color(0.96f, 0.96f, 0.96f));
				}
				// Create a texture for every testbox and store it for later drawing
				ci::gl::TextureRef Texture = ci::gl::Texture2d::create(textBox.render());
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
		return input;
	}

	std::vector<std::vector<ci::gl::TextureRef>> fieldTextures;
	ci::TextBox textBox;
	// Stores fields clicked on
	std::vector<std::vector<bool>> isBlack;
};
using BoardCreatorRef = std::shared_ptr<BoardCreator>;