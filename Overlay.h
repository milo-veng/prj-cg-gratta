#pragma once
#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>	
#include <gl\glut.h>											// Header File For The Glaux Library
#include <cstring>
#include <sstream>

class Overlay
{
public:
	int SCREEN_W;
	int SCREEN_H;

	//nota: sono float perchè li passa alle funzioni gl*f()
	float x1;			//ascissa di partenza della barra	
	float w_max;		//ascissa di arrivo della barra
	float h1,h2;		//h2-h1 = altezza della barra

public:
	Overlay(int SCREEN_W, int SCREEN_H);
	void drawLife(float life);


};

