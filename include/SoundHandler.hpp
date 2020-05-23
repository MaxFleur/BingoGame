
// Handles the sound file used when winning the game, handles a sound file path
//
// Code provided by MaxFleur
// https://github.com/MaxFleur


#pragma once
#include "cinder/app/App.h"
#include "cinder/audio/Voice.h"

// SoundHandler class, handle a variable soundfilePath and play it if needed
class SoundHandler {

public:
	SoundHandler() {};
	~SoundHandler() {};

	// Determine the sound file path at the beginning of the game and set it's volume
	void handleSoundFilePath() {
		mVoice = audio::Voice::create(audio::load(ci::app::loadAsset("winningSound.wav")));
		mVoice->setVolume(1.0f);
	}

	void playSound() {
		// Play the file, stop it if it already runs
		if (mVoice->isPlaying())
			mVoice->stop();
		mVoice->start();
	}

private:
	audio::VoiceRef mVoice;
};
using SoundHandlerRef = std::shared_ptr<SoundHandler>;