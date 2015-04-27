#pragma once
#include <iostream>
#include <cstring>
#include <windows.h>	
#include "Sound.h"
#include "audiere.h"

using namespace std;
using namespace audiere;



Sound::Sound(){



}

bool Sound::LoadFile(string filename, AudioDevicePtr device)
{ 
	stream = OutputStreamPtr( OpenSound(device, filename.c_str(), true) );
  if (!stream) {
	  string str = "Sound::LoadFile(): impossibile caricare il file: ";
	  str += filename;
	  MessageBox( NULL, str.c_str(), "Efzrrore", MB_ICONWARNING );
	  return false;         // failure
  }
}


void Sound::playSound()
{
	if(!stream)
	{
		MessageBox( NULL, "Sound::playSound(): impossibile avviare il file", "Errore", MB_ICONWARNING );
	}

	if( stream->isPlaying() ) stream->reset();
	else stream->play();

}


//loop del suono
void Sound::playSoundRepeat() {
	if(!stream)
	{
		MessageBox( NULL, "Sound::playSound(): impossibile avviare il file", "Errore", MB_ICONWARNING );
	}

	if( stream->isPlaying() ) stream->reset();
	else {
		stream->play();
		stream->setRepeat(true);
	}

}

void Sound::stopSound()
{
	if(!stream)
	{
		MessageBox( NULL, "Sound::stopSound(): impossibile stoppare il file", "Errore", MB_ICONWARNING );
	}

	if( stream->isPlaying() ) stream->stop();
	else stream->reset();

}

bool Sound::isPlaying()
{
	if(!stream)
	{
		MessageBox( NULL, "Sound::isPlaying()", "Errore", MB_ICONWARNING );
	}

	if (stream->isPlaying())
		return true;
}

void Sound::reset()
{
	if(!stream)
	{
		MessageBox( NULL, "Sound::reset()", "Errore", MB_ICONWARNING );
	}

	stream->reset();
}

Sound::~Sound(void)
{
}



void Sound::setVolume(float v) {
	if (v < 0.0f || v > 1.0f)
		return;

	stream->setVolume(v);
}