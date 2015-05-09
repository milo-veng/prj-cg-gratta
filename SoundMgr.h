#pragma once
#include <iostream>
#include <cstring>
#include <map>
#include <windows.h>	
#include "audiere.h"
#include "Sound.h"
#include <vector>
#include <string>
using namespace std;
using namespace audiere;

class SoundMgr
{
	Sound backgroundMusic;
	string backgroundMusicFilename;
	AudioDevicePtr *device;
	map<string, Sound> mp;

public:
	SoundMgr();

	void setBackgroundMusic(string filename) { backgroundMusicFilename = filename; }
	void playBackgroundMusic(string filename);

	void stopBackgroundMusic();

	void loadSounds(vector<string> filenames, vector<string> soundnames);

	void play(string name);

	void stop(string name);


	void mute(bool muted);
	void setVolume(float vol);	//[0;1]

	
};

