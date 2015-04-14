#include "Text.h"
#pragma once
#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>	
#include <gl\glut.h>											// Header File For The Glaux Library
#include <cstring>
#include <sstream>


using namespace std;

#pragma comment( lib, "opengl32.lib" )								// Search For OpenGL32.lib While Linking ( NEW )
#pragma comment( lib, "glu32.lib" )									// Search For GLu32.lib While Linking    ( NEW )
#pragma comment( lib, "glaux.lib" )									// Search For GLaux.lib While Linking    ( NEW )
#pragma comment( lib, "glut32.lib" )

Text::Text(void *font, float r, float g, float b)
{
	this->font = font;
	this->r = r;
	this->g = g;
	this->b = b;

	this->alpha = 1.0f;
}

void Text:: setOrthographicProjection(int SCREEN_W, int SCREEN_H) {
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_W, SCREEN_H, 0);
	glMatrixMode(GL_MODELVIEW);

}

void Text::resetPerpectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


//disegna la stringa lettera per lettera
void Text::renderBitmapString(float x, float y, void *font, string text) {
    glPushMatrix();

	glColor3f( r, g, b );
    glRasterPos2f(x, y);

	for (unsigned c=0; c < text.size(); c++) {
		glutBitmapCharacter(this->font, text[c]);
	}

	glPopMatrix();
}



void Text:: drawText(string text, float x, float y, int SCREEN_W, int SCREEN_H){
    glPushMatrix();
    setOrthographicProjection(SCREEN_W, SCREEN_H);
    glLoadIdentity();
    glClearColor( 0, 0, 0, 0 );

    glColor4f(r, g, b, 1);

	int font = (int)GLUT_BITMAP_8_BY_13;
    renderBitmapString(x, y,(void *)font, text);

    resetPerpectiveProjection();
    glPopMatrix();
}


