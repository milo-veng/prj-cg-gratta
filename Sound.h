#pragma once
#include <iostream>
#include <cstring>
#include <windows.h>	
#include "audiere.h"
using namespace std;
using namespace audiere;

class Sound
{
	OutputStreamPtr stream;

public:

	Sound();

	bool LoadFile(string filename, AudioDevicePtr device);
	
	void playSound();			//play, se suono già in play fa reset e play
	void playSound2();          //&play, se suono già in play non fa niente
	void playSoundRepeat();		//play and loop

	void stopSound();

	bool isPlaying();

	void reset();

	void setVolume(float v);


	~Sound(void);
};

