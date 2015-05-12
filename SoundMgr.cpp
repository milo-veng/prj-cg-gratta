#include "SoundMgr.h"
#include <iostream>
#include <cstring>
#include <windows.h>	
#include <vector>
#include <map>
#include <exception>

#include "audiere.h"
#include "Sound.h"
#include "WindowMgr.h"

using namespace std;
using namespace audiere;

extern WindowMgr winMgr;

SoundMgr::SoundMgr()
{
	device = new AudioDevicePtr( OpenDevice() );
	if (!device) {
		MessageBox(NULL, "AudioDevicePtr", "Errore", MB_OK|MB_ICONWARNING);         // failure
	}
}

void SoundMgr::playBackgroundMusic(string filename)
{

	//se il gioco è in pausa non aggiorna
	if (winMgr.paused)
		return;
	
	if (backgroundMusic.LoadFile(filename, *device)) {
		backgroundMusic.playSound();
	}

}


void SoundMgr::playMenuBackgroundMusic() {
	//può partire solo quando il gioco è in pausa
	if (!winMgr.paused)
		return;

	if (menuBackgroundMusicFilename == "")
		return;

	if (menuBackgroundMusic.LoadFile(menuBackgroundMusicFilename, *device)) {
		menuBackgroundMusic.playSound();
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

void SoundMgr::stopMenuBackgroundMusic() {
	if (menuBackgroundMusic.isPlaying()) menuBackgroundMusic.stopSound();
	else menuBackgroundMusic.reset();
}


//noreset è false di default
void SoundMgr::play(string name, bool noreset)
{
	//se il gioco è in pausa non aggiorna
	if (winMgr.paused)
		return;

	//mp[name.c_str()].playSound();				//qst aggiunge la chiave se non è presente
	try {
		if (!noreset)
			mp.at(name.c_str()).playSound();			//qst non aggiunge una chiave non presente
		else
			mp.at(name.c_str()).playSound2();
	}
	catch (out_of_range e) {
		return;
	}
}

void SoundMgr::stop(string name)
{

	//mp[name.c_str()].stopSound();
	try {
		mp.at(name.c_str()).stopSound();			//qst non aggiunge una chiave non presente
	}
	catch (out_of_range e) {
		return;
	}

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