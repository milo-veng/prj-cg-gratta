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

	w_max = float(SCREEN_W)-5.0f;
	h1 = 20.0f;
	h2 = float(SCREEN_H) - 550.0f;
}

void Overlay::drawLife(int life)
{
	if (life < 0)
		life = 1;

	float w = (w_max*life) / 1000.0f;


	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPushMatrix(); // Store The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(10, SCREEN_W, SCREEN_H, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPushMatrix(); // Store The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	glTranslated(0, 0, 0); // draw some stuff (blended, textured, alpha tested quad etc.)*/


	glBegin(GL_QUADS);				// Draw The Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(560.0f, h2, 0.0f);		// Bottom Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(w, h2, 0.0f);				// Bottom Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(w, h1, 0.0f);				// Top Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(560.0f, h1, 0.0f);				// Top Left
	glEnd();

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPopMatrix(); // Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPopMatrix();

	
}
