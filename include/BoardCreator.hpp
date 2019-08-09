#pragma once

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCv.h"
#include "Randomizer.hpp"

class BoardCreator {
public:
	BoardCreator() {};
	~BoardCreator() {};

	cv::Mat createBoard(cv::Mat input, std::vector<std::string> entrys) {

		// Clear size of textboxes and reserve them 25 fields
		texturesFromTextBoxes.clear();
		texturesFromTextBoxes.reserve(25);
		isBlack.clear();

		bfT.setAlignment(ci::TextBox::CENTER);
		bfT.setSize(ci::ivec2(157, 157));

		// Iterates over the board, creating textboxes and square meshes.
		int height = 100;
		int cloneIndex = 0;
		for (int x = 0; x <= 4; x++) {
			int width = 50;
			texturesFromTextBoxes.push_back(std::vector<ci::gl::TextureRef>());
			isBlack.push_back(std::vector<bool>());

			for (int y = 0; y <= 4; y++) {

				int textSize;
				if (entrys.at(cloneIndex).length() > 30) {
					textSize = 27;
				}
				else {
					textSize = 32;
				}

				bfT.setFont(ci::Font("Helvetica", textSize));
				bfT.setText(entrys.at(cloneIndex));
				if (x == 2 && y == 2) {
					isBlack[x].push_back(true);
					bfT.setColor(ci::Color(0.96f, 0.96f, 0.96f));
					bfT.setBackgroundColor(ci::Color(0.03f, 0.03f, 0.03f));
				}
				else {
					isBlack[x].push_back(false);
					bfT.setColor(ci::Color(0.0f, 0.0f, 0.0f));
					bfT.setBackgroundColor(ci::Color(0.96f, 0.96f, 0.96f));
				}
				// Create a texture for every testbox and store it for later drawing
				ci::gl::TextureRef Texture = ci::gl::Texture2d::create(bfT.render());
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

	std::vector<std::vector<ci::gl::TextureRef>> texturesFromTextBoxes;
	ci::TextBox bfT;
	std::vector<std::vector<bool>> isBlack;
};
using BoardCreatorRef = std::shared_ptr<BoardCreator>;