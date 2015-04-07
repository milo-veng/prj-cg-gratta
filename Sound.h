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
	
	void playSound();

	void stopSound();

	bool isPlaying();

	void reset();



	~Sound(void);
};

