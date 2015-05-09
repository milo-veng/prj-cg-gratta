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
#include <vector>
#include "SOIL.h"
#include "BoundingBox2D.h"
using namespace std;

extern ofstream logFile;

class MenuMgr
{
	int SCREEN_W;
	int SCREEN_H;

	GLuint mainMenuTex;
	GLuint gameoverTex;
	GLuint winTex;
	GLuint helpTex;

	string mainMenuTexFilename;
	string gameoverTexFilename;
	string winTexFilename;
	string helpTexFilename;

	void draw(int i);	//funz. disegno menu condivisa, NOTA che draw() pubblica non prende argomenti!
	void drawMenuItem(int menuItemNum, bool mouseOver);	//disegna gli elementi selezionabili del menu

	//disegnano i rispettivi menu
	void drawMainMenu();
	void drawGameOver() { draw(2); resetShowing();  showingGAMEOVER = true; }
	void drawWin() { draw(3); resetShowing();  showingWin = true; }
	void drawHelp() { draw(4); resetShowing(); showingHelp = true; }
	void drawGame();



	static const int MAINMENU_ITEMS_NUM = 3;			//# di item  che compongono main menu
	int selectedMenuItem;								//item del menu su cui è attiva la selezione(o mouseover),// 0 - GIOCA , 1- HELP, 2 - ESCI
	GLuint mainMenuItemsTex[2 * MAINMENU_ITEMS_NUM];					//scritte, i = scritta, i+1 = scritta_hover
	vector<BoundingBox2D> mainMenuItemsSize;			//posizione (x,y) dell'angolo sx e w e h per ciascun item del menu - sono le dim. della texture
	float timeSinceLastKeypress;						//delta t dall'ultimo tasto premuto e passato a manageKeyPressed


public:
	MenuMgr(int screenW, int screenH);
	~MenuMgr();

	bool loadTextures(string main, string gameover, string win, string help, vector<string> menuItems);
	

	bool draw();			//se l'utente si  trova in un menu disegna menu, altrimenti non fa niente e ritorna false


	//gestisce input utente quando si trova in un menu
	void manageKeyPressed(bool *keys, int size, float deltaT);
	void manageMouseMovedOrPressed(POINTS p);


	//quando sono true vengono mostrati i risp. menu
	bool showingMainMenu;	//parte mostrando il menu
	bool showingGAMEOVER;
	bool showingWin;
	bool showingHelp;		//mostrandop help
	void resetShowing() { showingMainMenu = false; showingGAMEOVER = false; showingWin = false; showingHelp = false; }	//mette a false tutti i showing*

};

