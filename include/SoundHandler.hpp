
// Handles the sound file used when winning the game, handles a sound file path
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include <Windows.h>
#include <mmsystem.h>

// SoundHandler class, handle a variable soundfilePath and play it if needed
class SoundHandler {

public:
	SoundHandler() {};
	~SoundHandler() {};

	// Determine the sound file path at the beginning of the game
	void handleSoundFilePath() {
		// Get the file path of the process
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		// Make a string out of it
		std::wstring w_buffer(buffer);
		std::string exePath(w_buffer.begin(), w_buffer.end());
		// Remove the "BingoGame.exe" with length of 13 out of the filename
		exePath = exePath.substr(0, exePath.size() - 13);
		// Append the directory of the sound file
		exePath.append("assets\\winningSound.wav");
		m_exePath = exePath;
	}

	void playSound() {
		// Convert string to const wchar_t and play sound
		std::wstring w_exePath = std::wstring(m_exePath.begin(), m_exePath.end());
		PlaySound(w_exePath.c_str(), NULL, SND_ASYNC);
	}

private:
	std::string m_exePath;
};
using SoundHandlerRef = std::shared_ptr<SoundHandler>;