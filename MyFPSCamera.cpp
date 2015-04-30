#include <ctime>
#include <cmath>
using namespace std;


#include "MyFPSCamera.h"
#include "BoundingBox2D.h"



MyFPSCamera::MyFPSCamera(void) {

		PIGRECO = 3.14159265;

		xpos  = 0.0; zpos = 0.0f; //posiz. sul terreno
		ypos = -2.0f; //altezza della telecamera( 1.0 è circa sul terreno)
		

		anglex = angley = anglez = 0.0;

		lx = ly = lz = 0.0;


		/* valori camera */
		cameraMovementspeed = 25;
		cameraRunningSpeedMult = 2; //runnigspeed = cameraMovSpeed*10
		cameraRotationSpeed = 2.5;
		cameraStrafeSpeed = 25;
		cameraVerticalSpeed = 0.05;

		oldMouseX = oldMouseY = 0.0;
		mouseDeltaX = mouseDeltaY = 0.0;
		mousePosReset = false;

		running = false;


		collisionsEnabled = true;
		playerW = 5.0;
		playerH = 5.0;

		//lastUpdate = clock();

}


//aggiorna i valori di mouseDeltaX e mouseDeltaY
//passo dei long int perchè la struttura POINTS delle WINANPI usa LONG = long int
void MyFPSCamera::updateMouseDeltaPos(long x, long y) {
	//calcolo di quanto si è spostato il mouse in un frame
	mouseDeltaX = x - oldMouseX;
	mouseDeltaY = y - oldMouseY;

	oldMouseX = x;
	oldMouseY = y;

	//sposto la telecamera di conseguenza
	if (mouseDeltaX > 0) {
		//giro verso dx
		mouseRotateRight();
	}
	else if(mouseDeltaX < 0) {
		//giro verso sx
		mouseRotateLeft();
	}

	if (mouseDeltaY > 0) {
		//guardo in alto
		mouseRotateUp();
	}
	else if (mouseDeltaY < 0) {
		//guardo in basso
		mouseRotateDown();
	}

}


//riposiziona il cursore del mouse al centro dello schermo
//serve per usare il mouse per muovere la telecamera in stile fps(GetWindowRect(hWnd, &r);
void MyFPSCamera::resetCursorPos(RECT windowRect, POINTS mousePos, int SCREEN_W, int SCREEN_H) {

	int windowCenterX = windowRect.left + SCREEN_W / 2;
	int windowCenterY = windowRect.top + SCREEN_H / 2;

	//string debug = "r.left=" + to_string(r.left) + ", r.top=" + to_string(r.top);
	//OutputDebugString( debug.c_str() );

	//centratura orizzontale
	if (mousePos.x < windowRect.left + 250 || mousePos.x > windowRect.right - 250) {
		SetCursorPos(windowCenterX, windowCenterY);
		mousePosReset = true;
	}

	//centratura verticale
	if ( mousePos.y < windowRect.top + 150 || mousePos.y > windowRect.bottom - 150 ) {
		SetCursorPos(windowCenterX, windowCenterY);
		mousePosReset = true;
	}

	//OutputDebugString("resetCursorPos\n");

}

//rotazioni
void MyFPSCamera::rotateLeft(double deltaT) {
			//rotazione risp. asse y - YAW
			if( angley + cameraRotationSpeed*deltaT > 2*PIGRECO )
				angley = 0.0;

			
			angley += cameraRotationSpeed*deltaT;
			
			lx = cos( angley );
			lz = -sin( angley );

}

void MyFPSCamera::rotateRight(double deltaT) {
			//rotaz. risp asse y - YAW
			if( angley - cameraRotationSpeed*deltaT < 0 ) 
				angley = 2*PIGRECO;

			angley -= cameraRotationSpeed*deltaT;

			lx = cos( angley );
			lz = -sin( angley );

}

//ruota a sx la telecamera in modo proporzionale a mouseDeltaX
void MyFPSCamera::mouseRotateLeft() {
	//angolo di cui ruotare la telecamera(yaw)
	double rotationAngle = cameraRotationSpeed*0.01;


	//rotazione risp. asse y - YAW
	//if (angley + rotationAngle > 2 * PIGRECO)
	//	angley = 0.0;


	angley += rotationAngle;

	lx = cos(angley);
	lz = -sin(angley);

}


void MyFPSCamera::mouseRotateRight() {
	//angolo di cui ruotare la telecamera(yaw)
	double rotationAngle = cameraRotationSpeed*0.01;

	//rotaz. risp asse y - YAW
	//if (angley - rotationAngle < 0)
	//	angley = 2 * PIGRECO;


	angley -= rotationAngle;

	lx = cos(angley);
	lz = -sin(angley);
}


//guarda in alto con il mouse
void MyFPSCamera::mouseRotateUp() {
	//angolo di cui ruotare la telecamera(pitch)
	double rotationAngle = cameraRotationSpeed*0.01;

	//rotaz. risp asse y - YAW
	if (anglez - rotationAngle < -PIGRECO/2.0 )
		anglez = -PIGRECO/2.0;

	anglez -= rotationAngle;
	//string debug = "rotataUP: " + std::to_string(anglez) + "\n";
	//OutputDebugString(debug.c_str());

	ly = sin(anglez);
}

//guarda in basso con il mouse
void MyFPSCamera::mouseRotateDown() {
	//angolo di cui ruotare la telecamera(pitch)
	double rotationAngle = cameraRotationSpeed*0.01;

	//rotaz. risp asse y - YAW
	if (anglez + rotationAngle > PIGRECO / 2.0)
		anglez = PIGRECO / 2.0;

	anglez += rotationAngle;
	//string debug = "rotataDOWN: " + std::to_string(anglez) + "\n";
	//OutputDebugString(debug.c_str());

	ly = sin(anglez);
}

	
//avanti, indietro
void MyFPSCamera::moveForward(double deltaT) {

	// di qnto mi sposterei nel prossimo frame rispetto alla posizione attuale nel caso mi muovessi
	double xDelta = lx*cameraMovementspeed*deltaT;
	double zDelta = lz*cameraMovementspeed*deltaT;

	if( collisionsEnabled ) {
		//creo boundingBox2D con la posiz. attuale del giocatore
		//BoundingBox2D player(tmpXPos, tmpZPos, playerW, playerH ); 
		BoundingBox2D player(xpos+xDelta, zpos+zDelta, playerW, playerH );	//predice la prossima posizione del player



		//Collisioni con la mappa
		//BoundingBox2D collider = terrain->isCollidingWith( player );
		BoundingBox2D collider = levelsMgr->get()->getTerrainModel()->isCollidingWith(player);

		//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
		//altrimenti isNull() è true
		if( !collider.isNull() ) {

			//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
			xDelta = zDelta = 0.0;

			sndMgr->play("COLLISION");

		}



		//Collisioni con le gemme e gestione di esse
		//la gestione di un eventuale collisione avvenuta è dentro checkCollisions()
		//BoundingBox2D gemCollider = objMgr->checkCollisions(player);
		BoundingBox2D gemCollider = levelsMgr->get()->getPickableObjMgr()->checkCollisions(player);

	} //fine collisioni


	//update posiz.
	xpos += xDelta;
	zpos += zDelta;
	
}

void MyFPSCamera::moveBackward(double deltaT) {
	double xDelta = -lx*cameraMovementspeed*deltaT;
	double zDelta = -lz*cameraMovementspeed*deltaT;



	if( collisionsEnabled ) {
		//creo boundingBox2D con la posiz. attuale del giocatore
		//BoundingBox2D player(tmpXPos, tmpZPos, playerW, playerH ); 
		BoundingBox2D player(xpos+xDelta, zpos+zDelta, playerW, playerH );	//predice la prossima posizione del player

		//verifica se ci sono collisioni
		//BoundingBox2D collider = terrain->isCollidingWith( player );
		BoundingBox2D collider = levelsMgr->get()->getTerrainModel()->isCollidingWith(player);

		//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
		//altrimenti isNull() è true
		if( !collider.isNull() ) {

			//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
			xDelta = zDelta = 0.0;

			sndMgr->play("COLLISION");

		}


		//Collisioni con le gemme e gestione di esse
		//la gestione di un eventuale collisione avvenuta è dentro checkCollisions()
		//BoundingBox2D gemCollider = objMgr->checkCollisions(player);
		BoundingBox2D gemCollider = levelsMgr->get()->getPickableObjMgr()->checkCollisions(player);

	}

	//update posiz.
	xpos += xDelta,
	zpos += zDelta;

}

//strafe laterale
void MyFPSCamera::strafeLeft(double deltaT) {
			//componenti della direzione in qui punta la telecamera
			lx = cos( angley );
			lz = -sin( angley );

			//uno dei due vettori perpendicolari avrà  componenti
			double normalToCameraDirectionX = -lz;
			double normalToCameraDirectionZ = lx;

			//strafe a sx
			double xDelta = -normalToCameraDirectionX*cameraStrafeSpeed*deltaT;
			double zDelta = -normalToCameraDirectionZ*cameraStrafeSpeed*deltaT;
			//xpos -= normalToCameraDirectionX*cameraStrafeSpeed*deltaT;
			//zpos -= normalToCameraDirectionZ*cameraStrafeSpeed*deltaT;


			if( collisionsEnabled ) {
				//creo boundingBox2D con la posiz. attuale del giocatore
				//BoundingBox2D player(tmpXPos, tmpZPos, playerW, playerH ); 
				BoundingBox2D player(xpos+xDelta, zpos+zDelta, playerW, playerH );	//predice la prossima posizione del player

				//verifica se ci sono collisioni
				//BoundingBox2D collider = terrain->isCollidingWith( player );
				BoundingBox2D collider = levelsMgr->get()->getTerrainModel()->isCollidingWith(player);

				//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
				//altrimenti isNull() è true
				if( !collider.isNull() ) {

					//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
					xDelta = zDelta = 0.0;

					sndMgr->play("COLLISION");

				}

				//Collisioni con le gemme e gestione di esse
				//la gestione di un eventuale collisione avvenuta è dentro checkCollisions()
				//BoundingBox2D gemCollider = objMgr->checkCollisions(player);
				BoundingBox2D gemCollider = levelsMgr->get()->getPickableObjMgr()->checkCollisions(player);

			}

			//update posiz.
			xpos += xDelta;
			zpos += zDelta;

}

void MyFPSCamera::strafeRight(double deltaT) {
		//componenti della direzione in qui punta la telecamera
			lx = cos( angley );
			lz = -sin( angley );

			//uno dei due vettori perpendicolari avrà  componenti
			double normalToCameraDirectionX = -lz;
			double normalToCameraDirectionZ = lx;

			//strafe a sx
			double xDelta = normalToCameraDirectionX*cameraStrafeSpeed*deltaT;
			double zDelta = normalToCameraDirectionZ*cameraStrafeSpeed*deltaT;
			//xpos += normalToCameraDirectionX*cameraStrafeSpeed*deltaT;
			//zpos += normalToCameraDirectionZ*cameraStrafeSpeed*deltaT;


			if( collisionsEnabled ) {
				//creo boundingBox2D con la posiz. attuale del giocatore
				//BoundingBox2D player(tmpXPos, tmpZPos, playerW, playerH ); 
				BoundingBox2D player(xpos+xDelta, zpos+zDelta, playerW, playerH );	//predice la prossima posizione del player

				//verifica se ci sono collisioni
				//BoundingBox2D collider = terrain->isCollidingWith( player );
				BoundingBox2D collider = levelsMgr->get()->getTerrainModel()->isCollidingWith(player);

				//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
				//altrimenti isNull() è true
				if( !collider.isNull() ) {

					//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
					xDelta = zDelta = 0.0;

					sndMgr->play("COLLISION");

				}

				//Collisioni con le gemme e gestione di esse
				//la gestione di un eventuale collisione avvenuta è dentro checkCollisions()
				//BoundingBox2D gemCollider = objMgr->checkCollisions(player);
				BoundingBox2D gemCollider = levelsMgr->get()->getPickableObjMgr()->checkCollisions(player);

			}

			//update posiz.
			xpos += xDelta;
			zpos += zDelta;	
}


//mov. verticale
void MyFPSCamera::moveUp(double deltaT) {
	ypos -= cameraMovementspeed*deltaT;
}

void MyFPSCamera::moveDown(double deltaT) {
	ypos += cameraMovementspeed*deltaT;
}


//disegna la BB della camera
void MyFPSCamera::drawBoundingBox() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//wireframe


	BoundingBox2D *b = &getBoundingBox();
	
	glBegin(GL_QUADS);
	glVertex3f(b->x, 0.0f, b->z);
	glVertex3f(b->x + b->w, 0.0f, b->z);
	glVertex3f(b->x + b->w, 0.0f, b->z + b->h);
	glVertex3f(b->x, 0.0f, b->z + b->h);
	glEnd();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //filled
}



