#include "Overlay.h"
#pragma once
#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>	
#include <gl\glut.h>											// Header File For The Glaux Library
#include <cstring>
#include <sstream>


Overlay::Overlay(int SCREEN_W, int SCREEN_H)
{
	this->SCREEN_W = SCREEN_W;
	this->SCREEN_H = SCREEN_H;

	//lunghezza barra
	x1 = SCREEN_W / 2 + SCREEN_W / 10;
	w_max = SCREEN_W - SCREEN_W / 25.0f;

	//altezza barra
	h1 = SCREEN_H / 60.0f;				//l'altezza barra è prop. all'altezza finestra
	h2 = h1 + SCREEN_H / 15.0f;			


	//iniz. oggetto per disegno del # maschere raccolte
	akus = NULL;
	akus = new AkuAku(SCREEN_W, SCREEN_H);
	
}

void Overlay::drawLife(float life)
{
	if (life <= 0.0f)
		life = 1.0f;

	float w = (w_max*life) / 100.0f;
	
	//glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPushMatrix(); // Store The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(10, SCREEN_W, SCREEN_H, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPushMatrix(); // Store The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	glTranslated(0, 0, 0); // draw some stuff (blended, textured, alpha tested quad etc.)*/


	glBegin(GL_QUADS);				// Draw The Quad
	 glVertex3f(x1, h2, 0.0f);		// Bottom Left
	 glVertex3f(w, h2, 0.0f);				// Bottom Right
	 glVertex3f(w, h1, 0.0f);				// Top Right
	 glVertex3f(x1, h1, 0.0f);				// Top Left
	glEnd();

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPopMatrix(); // Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPopMatrix();

	//glEnable(GL_LIGHTING);
	
}
