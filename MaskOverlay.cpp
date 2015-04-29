#include "MaskOverlay.h"
#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>	
#include <gl\glut.h>											// Header File For The Glaux Library
#include <string>
#include <sstream>
using namespace std;

#include "SOIL.h"

AkuAku::AkuAku(int SCREEN_W, int SCREEN_H)
{
	this->SCREEN_H = SCREEN_H;
	this->SCREEN_W = SCREEN_W;

	mask = 0;
}

int AkuAku::LoadGLTextures(string filename1, string filename2)
{
	logFile << "AkuAku::LoadGLTextures(f1,f2)...";

	/* load an image file directly as a new OpenGL texture */
	textureAkuBW = SOIL_load_OGL_texture
		(
		filename1.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);

	textureAkuC = SOIL_load_OGL_texture
		(
		filename2.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);


	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, textureAkuBW);
	glBindTexture(GL_TEXTURE_2D, textureAkuC);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	logFile << "OK" << endl;

	return true;									// Return Success
}



int AkuAku::init(string filename1,string filename2)
{
	if(!LoadGLTextures(filename1, filename2)) 
		MessageBox(NULL, "AkuAku::init(): immagine akuaku non caricata!","Errore", MB_OK);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	


	return TRUE;													// Initialization Went OK
}

void AkuAku:: draw(float a, float b, float c, float d)
{

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPushMatrix(); // Store The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(10, SCREEN_W, SCREEN_H, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPushMatrix(); // Store The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	glTranslated(0, 0, 0); // draw some stuff (blended, textured, alpha tested quad etc.)*/

	glBegin(GL_QUADS);				// Draw The Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(a, b, 0.0f);		// Bottom Left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(c, b, 0.0f);				// Bottom Right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(c, d, 0.0f);				// Top Right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(a, d, 0.0f);				// Top Left
	glEnd();


	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPopMatrix(); // Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPopMatrix();

}


void AkuAku:: drawmask()
{

	switch (mask)
	{
		case 0:
			glBindTexture(GL_TEXTURE_2D,textureAkuBW);
			draw(50.0f, 550.0f, 126.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuBW);
			draw(150.0f, 550.0f, 226.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuBW);
			draw(250.0f, 550.0f, 326.0f, 445.0f);
			break;

		case 1:
			glBindTexture(GL_TEXTURE_2D,textureAkuC);
			draw(50.0f, 550.0f, 126.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuBW);
			draw(150.0f, 550.0f, 226.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuBW);
			draw(250.0f, 550.0f, 326.0f, 445.0f);
			break;

		case 2:
			glBindTexture(GL_TEXTURE_2D,textureAkuC);
			draw(50.0f, 550.0f, 126.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuC);
			draw(150.0f, 550.0f, 226.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuBW);
			draw(250.0f, 550.0f, 326.0f, 445.0f);
			break;
		
		case 3:
			glBindTexture(GL_TEXTURE_2D,textureAkuC);
			draw(50.0f, 550.0f, 126.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuC);
			draw(150.0f, 550.0f, 226.0f, 445.0f);
			glBindTexture(GL_TEXTURE_2D,textureAkuC);
			draw(250.0f, 550.0f, 326.0f, 445.0f);
			break;
	}
}


void AkuAku:: maskPicked()
{
	if ( mask < 3)
		mask++;
}

void AkuAku:: maskDelete()
{
	if (mask > 0)
		mask--;
}
