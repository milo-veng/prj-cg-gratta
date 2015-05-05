#include "MenuMgr.h"

#include "WindowMgr.h"
extern WindowMgr winMgr;

MenuMgr::MenuMgr(int screenW, int screenH)
{
	SCREEN_W = screenW;
	SCREEN_H = screenH;

	//quando sono true vengono mostrati i risp. menu
	showingMainMenu = true;	//parte mostrando il menu
	showingGAMEOVER = false;
	showingWin = false;

	selectedMenuItem = 0; // 0 - GIOCA , 1- HELP, 2 - ESCI
}


MenuMgr::~MenuMgr()
{
}


bool MenuMgr::loadTextures(string main, string gameover, string win, string help, vector<string> menuItems) {


	mainMenuTexFilename = main;
	gameoverTexFilename = gameover;
	winTexFilename = win;

	logFile << "MenuMgr::loadTextures(f1,f2,f3)...";

	/* load an image file directly as a new OpenGL texture */
	mainMenuTex = SOIL_load_OGL_texture
		(
		mainMenuTexFilename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		NULL
		);

	gameoverTex = SOIL_load_OGL_texture
		(
		gameoverTexFilename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		NULL
		);

	winTex = SOIL_load_OGL_texture
		(
		winTexFilename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		NULL /*SOIL_FLAG_INVERT_Y*/
		);


	//caricoc tex. menuitems, i = scritta, i+1 = scritta HOVER
	for (int i = 0; i < menuItems.size(); i++) {
		mainMenuItemsTex[i] = SOIL_load_OGL_texture
			(
			menuItems.at(i).c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			NULL /*SOIL_FLAG_INVERT_Y*/
			);

	}


	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, mainMenuTex);
	glBindTexture(GL_TEXTURE_2D, gameoverTex);
	glBindTexture(GL_TEXTURE_2D, winTex);

	for (int i = 0; i < menuItems.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, mainMenuItemsTex[i]);
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	logFile << "OK" << endl;
	return true;
}


void MenuMgr::draw(int i) {

	switch (i) {
	case 1:
		glBindTexture(GL_TEXTURE_2D, mainMenuTex);
		break;
	case 2:
		glBindTexture(GL_TEXTURE_2D, gameoverTex);
		break;
	case 3:
		glBindTexture(GL_TEXTURE_2D, winTex);
		break;
	}
	 
	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPushMatrix(); // Store The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(10, SCREEN_W, SCREEN_H, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPushMatrix(); // Store The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	glTranslated(0, 0, 0); // draw some stuff (blended, textured, alpha tested quad etc.)*/

	glBegin(GL_QUADS);				// Draw The Quad	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0.0f);		// Bottom Left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(SCREEN_W, 0, 0.0f);				// Bottom Right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(SCREEN_W, SCREEN_H, 0.0f);				// Top Right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0,SCREEN_H, 0.0f);				// Top Left
	glEnd();


	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPopMatrix(); // Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPopMatrix();
}



void MenuMgr::drawMenuItem(int menuItemNum, int x, int y, bool mouseOver) {

	//disegna un quad di certe dimensioni nella posiz. specificata con o meno mouseover
	//...

}


//se l'utente si trova in un menu lo disegna e rit. true
//altrim. non fa niente e rit. false
bool MenuMgr::draw() {

	if (!showingGAMEOVER && !showingMainMenu && !showingWin)
		return false;


	if (showingMainMenu)
		drawMainMenu();
	else if (showingGAMEOVER)
		drawGameOver();
	else if (showingWin)
		drawWin();

	return true;
}



//gestisce i tasti premuti dall'utente
void MenuMgr::manageKeyPressed(bool *keys, int size) {

	//sto disegnando game, no menu
	if (!showingMainMenu && !showingGAMEOVER && !showingWin)
		return;



	//sono nel Main menu
	if (showingMainMenu) {

		if (keys[VK_UP]) {

			//selez. elem. sopra, se primo non va + su di così
			if (selectedMenuItem == 1 || selectedMenuItem == 2)
				selectedMenuItem--;

		}
		else if (keys[VK_DOWN]) {

			if (selectedMenuItem == 0 || selectedMenuItem == 1)
				selectedMenuItem++;
		
		}
		else if (keys[VK_RETURN]) {
			
			if (selectedMenuItem == 0)
				drawGame();
			else if (selectedMenuItem == 1)
				drawHelp();
			else if (selectedMenuItem == 2)
				winMgr.quit();
		}


	}
	else if (showingGAMEOVER) {
		//qls tasto si prema si finisce nel main menu
		showingGAMEOVER = false;
		showingMainMenu = true;
	}
	else if (showingWin) {
		//qls tasto si prema si finisce nel main menu
		showingWin = false;
		showingMainMenu = true;
	}

}


//gestisce movim. mouse e tasto sx
void MenuMgr::manageMouseMovedOrPressed(POINTS p) {

	//...

}

