/*
 *		This Code Was Created By Brett Porter For NeHe Productions 2000
 *		Visit NeHe Productions At http://nehe.gamedev.net
 *		
 *		Visit Brett Porter's Web Page at
 *		http://www.geocities.com/brettporter/programming
 */

#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>												// Header File For The Glaux Library
#include <cmath>
#include <cstring>
#include <fstream>
#include <ctime>
#include <ios>
#include <random>
using namespace std;


#include "WindowMgr.h"
#include "loadGLTexture.h"
#include "deltaT.h"
#include "MilkshapeModel.h"											// Header File For Milkshape Fil
#include "TerrainModel.h"
#include "SoundMgr.h" 
#include "Level.h"
#include "LevelsMgr.h"
#include "PickableObjectsManager.h"
#include "MyFPSCamera.h"											//header per la telecamera
#include "PlayerStats.h"
#include "Text.h"
#include "Enemy.h"
#include "MenuMgr.h"

 
#pragma comment( lib, "opengl32.lib" )								// Search For OpenGL32.lib While Linking ( NEW )
#pragma comment( lib, "glu32.lib" )									// Search For GLu32.lib While Linking    ( NEW )
#pragma comment( lib, "glaux.lib" )									// Search For GLaux.lib While Linking    ( NEW )

                                                                                                                                           


HDC			hDC=NULL;												// Private GDI Device Context
HGLRC		hRC=NULL;												// Permanent Rendering Context
HWND		hWnd=NULL;												// Holds Our Window Handle
HINSTANCE	hInstance;												// Holds The Instance Of The Application

//impostazioni finestra
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int SCREEN_BIT = 16;

bool keys[256];													// Array Used For The Keyboard Routine
bool active=TRUE;												// Window Active Flag Set To TRUE By Default
bool fullscreen=TRUE;											// Fullscreen Flag Set To Fullscreen Mode By Default



WindowMgr winMgr;												//Crea, ridim, disegna distrugge finestra

MyFPSCamera camera;												//telecamera in 1 persona stile FPS + collisioni
	
SoundMgr *sndMgr;												//suoni gioco e musica background

Text *txt;														//scrittura testo su schermo

LevelsMgr *levelsMgr;											//carica livelli: mappa, skybox, gemme, akuaku, bkgnd music

PlayerStats pStats(winMgr.SCREEN_W, winMgr.SCREEN_H);			//punteggio, progressi giocatore

extern clock_t lastUpdate;										//usato per calcolare deltaT tra frame e l'altro
double deltaT = 0;												//deltaT tra un frame e l'altro

POINTS p;														//posizione del mouse
ofstream logFile;												//file log

string fps;
int frameCounter;												//usato per calcolo fps, conta 100 frame renderizzati e poi torna a 0
float cumulativeDeltaT;											//tempo necessario per renderizzare 100 frame -> aggiorno fps ogni 100 frame 


Enemy e;
MenuMgr menu(SCREEN_W, SCREEN_H);


int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow)
{
	MSG		msg;													// Windows Message Structure										// Bool Variable To Exit Loop
	time_t currentTime;
	time(&currentTime);

	//apre il file di log
	logFile.open("cg-prj-log.txt", ios::out);
	if (!logFile.is_open()) {
		MessageBox(NULL, "Impossibile creare il file di log!", "Warning", MB_OK | MB_ICONWARNING);
	}

	logFile << "Progetto CG" << endl;
	logFile << asctime(localtime(&currentTime)) << endl;					//momento di avvio del gioco

	/* caricamento livello */
	levelsMgr = new LevelsMgr();
	levelsMgr->loadLevel(levelsMgr->LEVEL_1);								//carica mappa + skybox + gemme + mask + bkgnd music


	/* caricamento suoni */
	sndMgr = new SoundMgr();
	sndMgr->playBackgroundMusic("Data/audio/monkeyislandsecretsintro.mp3");	//Secrets of Monkey Island - Title
	vector<string> sounds; sounds.push_back("Data/audio/gem.wav"); sounds.push_back("Data/audio/budega.mp3"); sounds.push_back("Data/audio/collision.mp3"); //sounds contiene i nomi dei file
	vector<string> soundNames; soundNames.push_back("GEM"); soundNames.push_back("BUDEGA");	 soundNames.push_back("COLLISION");					//soundNames il nome di ciascun suono: sndMgr->play("NOME")
	sndMgr->loadSounds(sounds, soundNames);

	//carica modello fantasmino
	e.loadModelData("Data/gem.ms3d");

	// Create Our OpenGL Window
	logFile << "Creazione finestra" << endl;
	if (!winMgr.CreateGLWindow("Progetto CG"))
	{
		MessageBox(NULL, "Impossibile creare la finestra!", "Errore", MB_ICONERROR);
		return 0;													// Quit If Window Was Not Created
	}


	pStats.initMasksOverlay("Data/texture/Aku_Aku2.png", "Data/texture/Aku_Aku1.png");	//carica le immagini degli aku aku per l'overlay - va messo qui perchè deve già esistere un opengl context quando la chiamo


	//carico immagini menu e item del menu principale
	vector<string> f; f.push_back("Data/texture/GIOCA.png"); f.push_back("Data/texture/GIOCA_hover.png"); f.push_back("Data/texture/HELP.png"); f.push_back("Data/texture/HELP_hover.png"); f.push_back("Data/texture/ESCI.png"); f.push_back("Data/texture/ESCI_hover.png");
	menu.loadTextures("Data/bk.png", "Data/bk.png", "Data/bk.png", "Data/bk.png", f);

	ShowCursor(FALSE);												//nasconde punt. mouse

	txt = new Text(GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);		//iniz. testo a schermo
	
	lastUpdate = clock();											//iniz. timer per contare i tick tra un frame e l altro -> fps independent movement



	//agg. menu show a drawGLSCene()
	


	/* GAME LOOP */
	winMgr.gameLoop(msg, deltaT);




	winMgr.KillGLWindow();		
	return (msg.wParam);											// Exit The Program
}
