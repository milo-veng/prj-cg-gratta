#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <windows.h>	
#include "Sound.h"
#include "audiere.h"

using namespace std;
using namespace audiere;

extern ofstream logFile;

Sound::Sound(){

	stream = NULL;

}

bool Sound::LoadFile(string filename, AudioDevicePtr device)
{ 
	stream = OutputStreamPtr( OpenSound(device, filename.c_str(), true) );
  if (!stream) {
	  string str = "Sound::LoadFile(): impossibile caricare il file: ";
	  str += filename;
	  MessageBox( NULL, str.c_str(), "Efzrrore", MB_ICONWARNING );
	  logFile << str << endl;
	  return false;         // failure
  }
}


//se il suono è in play lo resetta quando viene chiamata
void Sound::playSound()
{
	if(stream==NULL)
	{
		MessageBox( NULL, "Sound::playSound(): impossibile avviare il file", "Errore", MB_ICONWARNING );
		return;
	}

	if( stream->isPlaying() ) stream->reset();
	else stream->play();

}

//se il suono è in play non fa niente quando viene chiamata(chiamate dai cicli)
void Sound::playSound2()
{
	if (stream == NULL)
	{
		MessageBox(NULL, "Sound::playSound(): impossibile avviare il file", "Errore", MB_ICONWARNING);
		return;
	}

	//if (stream->isPlaying()) return;
	//else stream->play();
	stream->play();
}


//loop del suono
void Sound::playSoundRepeat() {
	if(stream==NULL)
	{
		MessageBox( NULL, "Sound::playSound(): impossibile avviare il file", "Errore", MB_ICONWARNING );
		return;
	}

	if( stream->isPlaying() ) stream->reset();
	else {
		stream->play();
		stream->setRepeat(true);
	}

}

void Sound::stopSound()
{
	if(stream==NULL)
	{
		//MessageBox( NULL, "Sound::stopSound(): impossibile stoppare il file", "Errore", MB_ICONWARNING );
		return;
	}

	if( stream->isPlaying() ) stream->stop();
	else stream->reset();

}

bool Sound::isPlaying()
{
	if(stream==NULL)
	{
		MessageBox( NULL, "Sound::isPlaying()", "Errore", MB_ICONWARNING );
		return false;
	}

	if (stream->isPlaying())
		return true;
}

void Sound::reset()
{
	if(stream==NULL)
	{
		MessageBox( NULL, "Sound::reset()", "Errore", MB_ICONWARNING );
		return;
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