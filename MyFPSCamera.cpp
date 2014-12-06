#include <ctime>
#include <cmath>
using namespace std;


#include "MyFPSCamera.h"


MyFPSCamera::MyFPSCamera(void) {

		PIGRECO = 3.14159265;

		xpos  = 75; zpos = 1.0f; //posiz. sul terreno
		ypos = 4.5f; //altezza della telecamera( 1.0 è circa sul terreno)
		

		anglex = angley = anglez = 0.0;

		lx = ly = lz = 0.0;


		/* valori camera */
		cameraMovementspeed = 0.1;
		cameraRunningSpeedMult = 10; //runnigspeed = cameraMovSpeed*10
		cameraRotationSpeed = 0.05;
		cameraStrafeSpeed = 0.1;
		cameraVerticalSpeed = 0.05;

		mouseDeltaX = mouseDeltaY = 0.0;

		running = false;

		//lastUpdate = clock();
}



//rotazioni
void MyFPSCamera::rotateLeft(double deltaT) {
			//rotazione risp. asse y - YAW
			if( angley + cameraRotationSpeed > 2*PIGRECO )
				angley = 0.0;

			
			angley += cameraRotationSpeed;

			
			//lx = cos( angley )*deltaT;
			//lz = -sin( angley )*deltaT;

			lx = cos( angley );
			lz = -sin( angley );
}

void MyFPSCamera::rotateRight(double deltaT) {
			//rotaz. risp asse y - YAW
			if( angley - cameraRotationSpeed < 0 ) 
				angley = 2*PIGRECO;

			angley -= cameraRotationSpeed;


			//lx = cos( angley )*deltaT;
			//lz = -sin( angley )*deltaT;
			lx = cos( angley );
			lz = -sin( angley );

}

	
//avanti, indietro
void MyFPSCamera::moveForward(double deltaT) {
	xpos += lx*cameraMovementspeed;
	zpos += lz*cameraMovementspeed;
	//xpos += lx*cameraMovementspeed;
	//zpos += lz*cameraMovementspeed;
}

void MyFPSCamera::moveBackward(double deltaT) {
	xpos -= lx*cameraMovementspeed;
	zpos -= lz*cameraMovementspeed;
	//xpos -= lx*cameraMovementspeed*deltaT;
	//zpos -= lz*cameraMovementspeed*deltaT;
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
			xpos -= normalToCameraDirectionX*cameraStrafeSpeed;
			zpos -= normalToCameraDirectionZ*cameraStrafeSpeed;
			//xpos -= normalToCameraDirectionX*cameraStrafeSpeed*deltaT;
			//zpos -= normalToCameraDirectionZ*cameraStrafeSpeed*deltaT;

}

void MyFPSCamera::strafeRight(double deltaT) {
	//componenti della direzione in qui punta la telecamera
			lx = cos( angley );
			lz = -sin( angley );

			//uno dei due vettori perpendicolari avrà  componenti
			double normalToCameraDirectionX = -lz;
			double normalToCameraDirectionZ = lx;

			//strafe a sx
			xpos += normalToCameraDirectionX*cameraStrafeSpeed;
			zpos += normalToCameraDirectionZ*cameraStrafeSpeed;
			//xpos += normalToCameraDirectionX*cameraStrafeSpeed*deltaT;
			//zpos += normalToCameraDirectionZ*cameraStrafeSpeed*deltaT;
}


//mov. verticale
void MyFPSCamera::moveUp(double deltaT) {
	ypos -= cameraMovementspeed;
}

void MyFPSCamera::moveDown(double deltaT) {
	ypos += cameraMovementspeed;
}




