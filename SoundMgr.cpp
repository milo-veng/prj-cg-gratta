#include "SoundMgr.h"
#include <iostream>
#include <cstring>
#include <windows.h>	
#include "audiere.h"
#include "Sound.h"
#include <vector>
using namespace std;
using namespace audiere;


SoundMgr::SoundMgr()
{
	device = new AudioDevicePtr( OpenDevice() );
	if (!device) {
		MessageBox(NULL, "AudioDevicePtr", "Errore", MB_OK|MB_ICONWARNING);         // failure
	}
}

void SoundMgr::playBackgroundMusic(string filename)
{

	
	if( backgroundMusic.LoadFile(filename, *device) ) {
		backgroundMusic.playSound();
	}

}

void SoundMgr::stopBackgroundMusic()
{


	if( backgroundMusic.isPlaying() ) backgroundMusic.stopSound();
	else backgroundMusic.reset();

}

void SoundMgr::loadSounds(vector<string> filenames, vector<string> soundnames)
{
	for(int i=0; i<filenames.size(); i++) {
		Sound sound;
		sound.LoadFile(filenames.at(i), *device);
		mp.insert( std::make_pair( soundnames.at(i), sound) );
}

	}

void SoundMgr::play(string name)
{

	mp[name.c_str()].playSound();
}

void SoundMgr::stop(string name)
{

	mp[name.c_str()].stopSound();
}
		



void SoundMgr::mute(bool muted) {
	float vol = muted ? 0.0f : 1.0f;

	backgroundMusic.setVolume(vol);

	for (std::map<string, Sound>::iterator it = mp.begin(); it != mp.end(); ++it)
		it->second.setVolume(vol);

}

//[0;1]
void SoundMgr::setVolume(float vol) {

	if (vol < 0.0f || vol > 1.0f)
		return;


	backgroundMusic.setVolume(vol);

	for (std::map<string, Sound>::iterator it = mp.begin(); it != mp.end(); ++it)
		it->second.setVolume(vol);

}