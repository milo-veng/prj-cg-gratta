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

		mouseDeltaX = mouseDeltaY = 0.0;

		running = false;


		collisionsEnabled = true;
		playerW = 5.0;
		playerH = 5.0;

		//lastUpdate = clock();

}



//rotazioni
void MyFPSCamera::rotateLeft(double deltaT) {
			//rotazione risp. asse y - YAW
			if( angley + cameraRotationSpeed*deltaT > 2*PIGRECO )
				angley = 0.0;

			
			angley += cameraRotationSpeed*deltaT;

			
			//lx = cos( angley )*deltaT;
			//lz = -sin( angley )*deltaT;

			lx = cos( angley );
			lz = -sin( angley );

}

void MyFPSCamera::rotateRight(double deltaT) {
			//rotaz. risp asse y - YAW
			if( angley - cameraRotationSpeed*deltaT < 0 ) 
				angley = 2*PIGRECO;

			angley -= cameraRotationSpeed*deltaT;


			//lx = cos( angley )*deltaT;
			//lz = -sin( angley )*deltaT;
			lx = cos( angley );
			lz = -sin( angley );

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

		//verifica se ci sono collisioni
		BoundingBox2D collider = terrain->isCollidingWith( player );

		//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
		//altrimenti isNull() è true
		if( !collider.isNull() ) {

			//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
			xDelta = zDelta = 0.0;

		}

	}

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
		BoundingBox2D collider = terrain->isCollidingWith( player );

		//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
		//altrimenti isNull() è true
		if( !collider.isNull() ) {

			//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
			xDelta = zDelta = 0.0;

		}

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
				BoundingBox2D collider = terrain->isCollidingWith( player );

				//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
				//altrimenti isNull() è true
				if( !collider.isNull() ) {

					//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
					xDelta = zDelta = 0.0;

				}

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
				BoundingBox2D collider = terrain->isCollidingWith( player );

				//se c'è stata 1 collisione collider contiene la bounding box dell'ogg. con cui sto collidendo
				//altrimenti isNull() è true
				if( !collider.isNull() ) {

					//poichè c'è stata una collisione nella posizione predetta fermo il player dal proseguire in qst direzione
					xDelta = zDelta = 0.0;

				}

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




