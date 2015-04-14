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



class Text
{
	void *font;

	//colore
	float r, g, b;
	float alpha;			//default a 1

public:
	Text(void *font, float r, float g, float b);

	void setColor(int r, int g, int b) { this->r = r; this->g = g; this->b = b; }
	void setAlpha(float alpha) { this->alpha = alpha; }

	void setOrthographicProjection(int SCREEN_W, int SCREEN_H);	//x disegno 2D
	void resetPerpectiveProjection();							//resetta x il disegno 3D
	void Text::renderBitmapString(float x, float y, void *font, string text);

	//scrive il testo passato nella posizione (x,y) su una finestra delle dim. specificate
	void drawText(string text, float x, float y, int SCREEN_W, int SCREEN_H);




};


