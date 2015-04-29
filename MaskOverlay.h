#pragma once
#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>	
#include <gl\glut.h>											// Header File For The Glaux Library
#include <string>
#include <sstream>
#include <fstream>
#include "SOIL.h"
using namespace std;

extern ofstream logFile;

class AkuAku
{
public:
	GLuint textureAkuBW, textureAkuC;
	int SCREEN_W;
	int SCREEN_H;
	int mask;


	void draw(float a, float b, float c, float d);
	int LoadGLTextures(string filename1, string filename2);

public:

	AkuAku(int SCREEN_W, int SCREEN_H);
	
	int init(string filename1, string filename2);	//carica immagini	
	
	void drawmask();					//disegno a schermo

	//inc/dec # maschere raccolte
	void maskPicked();
	void maskDelete();
};

