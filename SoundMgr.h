#pragma once
#include <iostream>
#include <cstring>ù
#include <map>
#include <windows.h>	
#include "audiere.h"
#include "Sound.h"
#include <vector>
using namespace std;
using namespace audiere;

class SoundMgr
{
	Sound backgroundMusic;
	AudioDevicePtr *device;
	map<string, Sound> mp;

public:
	SoundMgr();

	void playBackgroundMusic(string filename);

	void stopBackgroundMusic();

	void loadSounds(vector<string> filenames, vector<string> soundnames);

	void play(string name);

	void stop(string name);


	void mute(bool muted);
	void setVolume(float vol);	//[0;1]

	
};

