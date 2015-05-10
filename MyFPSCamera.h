#pragma once

#include <Windows.h>
#include <ctime>
using namespace std;


#ifndef _MY_FPS_CAMERA_
#define _MY_FPS_CAMERA_

#include "TerrainModel.h"
#include "PickableObjectsManager.h"
#include "Level.h"
#include "LevelsMgr.h"
#include "Sound.h"
#include "SoundMgr.h"


extern SoundMgr *sndMgr;												//suoni gioco e musica background
extern LevelsMgr *levelsMgr;		


//posizione player(telecamera)
struct MyFPSCamera {
public:
	
	//posiz. camera nello spazioe
	double xpos;
	double ypos;
	double zpos;

	//rotaz. camera rispetto ai 3 assi
	double anglex;
	double angley;
	double anglez;

	//line of sight
	double lx;
	double ly;
	double lz;

	double cameraMovementspeed; //velocità movimento avanti/indietro
	double cameraRunningSpeedMult; //moltiplicatore della velocita movimento normale avanti/indietro quando si sta correndo
	double cameraRotationSpeed; //""		movimento di rotazione dx,sx
	double cameraStrafeSpeed;	//""		movimento laterale
	double cameraVerticalSpeed;	//x debug, non esiste movimento verticale nel gioco finito

	//movimento del mouse
	double oldMouseX;			//la pos. del mouse al frame precedente
	double oldMouseY;
	double mouseDeltaX;			//la funzione updateMouseDeltaPos(newPos) aggiorna il valore di mouseDeltaX e mouseDeltaY
	double mouseDeltaY;
	bool mousePosReset;			//true nel freame in cui faccio reset della posiz. mouse al centro dello schermo


	//se true il personaggio sta corrent
	bool running;

	
	//true se le collisioni camera-mondo sono attive
	bool collisionsEnabled;
	float playerW;				//larghezza(asseX) bounding box del player(camera)
	float playerH;				//altezza(asseZ) bounding box del player(camera)


	//usato per creare un movimento frameindependent
	//clock_t lastUpdate;


	double PIGRECO;



	MyFPSCamera();
	
	
	//ritorna la BB del player, Nota: non tiene conto del deltaPos del frame attuale!
	BoundingBox2D getBoundingBox() { BoundingBox2D bb(xpos-playerW/2.0f, zpos-playerH/2.0f, playerW, playerH); return bb; }

	//aggiorna mouseDeltaX e mouseDeltaY
	void updateMouseDeltaPos(long x, long y );

	//riposiziona il cursore al centro dello schermo(rotaz. telecamera col mouse)
	void resetCursorPos(RECT windowRect, POINTS mousePos, int SCREEN_W, int SCREEN_H);

	//disegna la BB della camera
	void drawBoundingBox();


	//rotazioni
	void rotateLeft(double deltaT);
	void rotateRight(double deltaT);
	void mouseRotateLeft();
	void mouseRotateRight();
	void mouseRotateUp();
	void mouseRotateDown();
	
	//avanti, indietro
	void moveForward(double deltaT);
	void moveBackward(double deltaT);

	//strafe laterale
	void strafeLeft(double deltaT);
	void strafeRight(double deltaT);


	//movimento verticale
	void moveUp(double deltaT);
	void moveDown(double deltaT);


	//imposta running true/false
	void run() { if( !running ) {cameraMovementspeed *= cameraRunningSpeedMult; running = true; } }

	void walk() { if(running) { cameraMovementspeed /= cameraRunningSpeedMult; running = false;} }



	//riposiziona camera nella posiz. inizale, chiamato quando ricarico livello dopo aver perso
	void resetCameraPos() {	anglex = angley = anglez = 0.0;	lx = ly = lz = 0.0; xpos = zpos = 0.0f; }

} ;

#endif