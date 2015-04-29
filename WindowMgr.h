#pragma once

#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>												// Header File For The Glaux Library
#include <cmath>
#include <cstring>
#include <fstream>
#include <ctime>
#include <ios>
using namespace std;

#include "MyFPSCamera.h"
#include "Level.h"
#include "LevelsMgr.h"
#include "Text.h"

extern ofstream logFile;
extern MyFPSCamera camera;
extern LevelsMgr *levelsMgr;
extern Text *txt;
extern PlayerStats pStats;
extern POINTS p;
extern double deltaT;




extern bool keys[256];													// Array Used For The Keyboard Routine
extern bool active;										// Window Active Flag Set To TRUE By Default
extern bool fullscreen;


extern string fps;
extern int frameCounter;	//usato per calcolo fps, conta 100 frame renderizzati e poi torna a 0
extern float cumulativeDeltaT; //deltaT per renderizzare 100 frame


class WindowMgr
{
public:
	//winapi
	HDC			hDC = NULL;												// Private GDI Device Context
	HGLRC		hRC = NULL;												// Permanent Rendering Context
	HWND		hWnd = NULL;												// Holds Our Window Handle
	HINSTANCE	hInstance;

	//impostazioni finestra
	const int SCREEN_W = 800;
	const int SCREEN_H = 600;
	const int SCREEN_BIT = 16;
									

	bool showFPS = TRUE;
	bool showCameraPosition = TRUE;


	WindowMgr();
	~WindowMgr();


	//processa messaggi destinati alla finestra
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	//crea finestra
	BOOL CreateGLWindow(char* title);
	GLvoid KillGLWindow(GLvoid);

	//ricarica texture e iniz. opengl
	int InitGL(GLvoid);

	//chiamata quando viene ridim. finestra
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

	//chiamata a ogni frame per disegnare
	int DrawGLScene(GLvoid);
	
	
	//MAIN LOOP DEL GIOCO
	void gameLoop(MSG &msg, double &deltaT);

	
};

