#pragma once
#include <Windows.h>
#include <mmsystem.h>

#include <iostream>
#include <string.h>


class SoundHandler {

public:
	SoundHandler() {};
	~SoundHandler() {};

	void handleSoundFilePath() {
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);

		std::wstring w_buffer(buffer);
		std::string exePath(w_buffer.begin(), w_buffer.end());

		const std::string substractStr("BingoGame.exe");
		exePath = exePath.substr(0, exePath.size() - substractStr.size());

		exePath.append("assets\\winningSound.wav");
		std::wstring w_exePath = std::wstring(exePath.begin(), exePath.end());
		m_soundFilePath = w_exePath.c_str();
	}

	void playSound() {
		PlaySound(m_soundFilePath, NULL, SND_ASYNC);
	}

private:
	const wchar_t* m_soundFilePath;
};
using SoundHandlerRef = std::shared_ptr<SoundHandler>;