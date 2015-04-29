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
using namespace std;

#include "MaskOverlay.h"


class Overlay
{
public:
	int SCREEN_W;
	int SCREEN_H;

	//nota: sono float perchè li passa alle funzioni gl*f()
	float x1;			//ascissa di partenza della barra	
	float w_max;		//ascissa di arrivo della barra
	float h1,h2;		//h2-h1 = altezza della barra

	AkuAku *akus;

public:
	Overlay(int SCREEN_W, int SCREEN_H);

	//disegna barra vita
	void drawLife(float life);

	//carica immagini maschere e crea texture
	void initMasks(string f1, string f2) { akus->init(f1, f2); }

	//disegna le maschere a schermo in overlay
	void drawMasks() { if (akus != NULL) { akus->drawmask(); } }

	//inc/dec # maschere raccolte
	void maskPicked() { akus->maskPicked(); }
	void removeMask() { akus->maskDelete(); }


};

