#pragma once

#include <Windows.h>
#include <ctime>
using namespace std;


#ifndef _MY_FPS_CAMERA_
#define _MY_FPS_CAMERA_

#include "TerrainModel.h"

extern TerrainModel *terrain;

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
	double mouseDeltaX;
	double mouseDeltaY;


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




	//rotazioni
	void rotateLeft(double deltaT);
	void rotateRight(double deltaT);
	
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

} ;

#endif