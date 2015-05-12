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
	Sound menuBackgroundMusic;
	string backgroundMusicFilename;
	string menuBackgroundMusicFilename;
	AudioDevicePtr *device;
	map<string, Sound> mp;

public:
	SoundMgr();

	void setBackgroundMusic(string filename) { backgroundMusicFilename = filename; }
	void setMenuBackgroundMusic(string f) { menuBackgroundMusicFilename = f; }
	void playBackgroundMusic(string filename);
	void playMenuBackgroundMusic();

	void stopBackgroundMusic();
	void stopMenuBackgroundMusic();

	void loadSounds(vector<string> filenames, vector<string> soundnames);

	void play(string name, bool noreset = false);

	void stop(string name);


	void mute(bool muted);
	void setVolume(float vol);	//[0;1]

	
};

