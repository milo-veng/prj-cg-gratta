#include "MenuMgr.h"

#include "WindowMgr.h"
#include "SoundMgr.h"

extern WindowMgr winMgr;
extern SoundMgr *sndMgr;

MenuMgr::MenuMgr(int screenW, int screenH)
{
	SCREEN_W = screenW;
	SCREEN_H = screenH;

	//quando sono true vengono mostrati i risp. menu
	showingMainMenu = true;	//parte mostrando il menu
	showingGAMEOVER = false;
	showingWin = false;
	showingHelp = false;

	selectedMenuItem = 0; // 0 - GIOCA , 1- HELP, 2 - ESCI

	mainMenuTex = gameoverTex = winTex = helpTex = 0;	//iniz. a 0

	//iniz. le dimensioni dei "tasti" del menu
	int playW = 192, playH = 61;
	int helpW = 165, helpH = 60;
	int esciW = 147, esciH = 61;

	//posiz. e dimensioni dei "tasti" del main menu
	BoundingBox2D playBB(SCREEN_W/2 - playW/2, 200, playW, playH);
	BoundingBox2D helpBB(SCREEN_W / 2 - helpW / 2, 300, helpW, helpH);
	BoundingBox2D esciBB(SCREEN_W/2 - esciW/2, 400, esciW, esciH);

	//li salvo
	mainMenuItemsSize.push_back(playBB);
	mainMenuItemsSize.push_back(helpBB);
	mainMenuItemsSize.push_back(esciBB);
}


MenuMgr::~MenuMgr()
{
}


bool MenuMgr::loadTextures(string main, string gameover, string win, string help, vector<string> menuItems) {


	mainMenuTexFilename = main;
	gameoverTexFilename = gameover;
	winTexFilename = win;
	helpTexFilename = help;

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

	helpTex = SOIL_load_OGL_texture
		(
		helpTexFilename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		NULL /*SOIL_FLAG_INVERT_Y*/
		);

	if (mainMenuTex == 0 || gameoverTex == 0 || winTex == 0 || helpTex == 0) {
		logFile << "MenuMgr::loadTextures(f1,f2,f3,f4,vector<string>): Impossibile caricare uno degli sfondi dei menu!" << endl;
		MessageBox(NULL, "MenuMgr::loadTextures(): uno o piu degli sfondi menu non caricato!", "Errore", MB_ICONWARNING);
	}


	//caricoc tex. menuitems, i = scritta, i+1 = scritta HOVER
	for (int i = 0; i < menuItems.size(); i++) {
		mainMenuItemsTex[i] = SOIL_load_OGL_texture
			(
			menuItems.at(i).c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			NULL /*SOIL_FLAG_INVERT_Y*/
			);

		if (mainMenuItemsTex[i] == 0) {
			logFile << "MenuMgr::loadTextures(): impossibile caricare uno o piu menu items!" << endl;
			MessageBox(NULL, "MenuMgr::loadTextures(): impossibile caricare elementi menu", "Errore", MB_ICONWARNING);
		}

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
	case 4:
		glBindTexture(GL_TEXTURE_2D, helpTex);
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



void MenuMgr::drawMenuItem(int menuItemNum, bool mouseOver) {

	//disegna un quad di certe dimensioni nella posiz. specificata con o meno mouseover
	
	// 0 - GIOCA , 1- HELP, 2 - ESCI
	//#############	l 'INDICE è sbagliato1!!!!!!!! ############### menuItemNum va corretto!
	if (!mouseOver)
		glBindTexture(GL_TEXTURE_2D, mainMenuItemsTex[menuItemNum*2]); //pulsante normale
	else
		glBindTexture(GL_TEXTURE_2D, mainMenuItemsTex[menuItemNum*2 + 1]); //pulsante selezionato

	BoundingBox2D item = mainMenuItemsSize.at(menuItemNum);

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPushMatrix(); // Store The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix
	glOrtho(10, SCREEN_W, SCREEN_H, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPushMatrix(); // Store The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	glTranslated(0, 0, 0); // draw some stuff (blended, textured, alpha tested quad etc.)*/

	
	glBegin(GL_QUADS);														// Draw The Quad	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(item.x, item.z, 0.0f);					// Bottom Left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(item.x + item.w, item.z, 0.0f);				// Bottom Right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(item.x + item.w, item.z + item.h, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(item.x, item.z + item.h, 0.0f);
					// Top Left
	glEnd();


	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glPopMatrix(); // Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glPopMatrix();

}


//se l'utente si trova in un menu lo disegna e rit. true
//altrim. non fa niente e rit. false
bool MenuMgr::draw() {

	if (!showingGAMEOVER && !showingMainMenu && !showingWin && !showingHelp)
		return false;


	if (showingMainMenu)
		drawMainMenu();
	else if (showingGAMEOVER)
		drawGameOver();
	else if (showingWin)
		drawWin();
	else if (showingHelp) {
		drawHelp();
	}

	return true;
}



//disegna sfondo main menu e menu items
void MenuMgr::drawMainMenu() {

	draw(1);					//disegna sfondo main menu
	
	showingMainMenu = true; 

	//disegna elementi menu principale)
	(selectedMenuItem == 0) ? drawMenuItem(0, true) : drawMenuItem(0, false);
	(selectedMenuItem == 1) ? drawMenuItem(1, true) : drawMenuItem(1, false);
	(selectedMenuItem == 2) ? drawMenuItem(2, true) : drawMenuItem(2, false);


}


//disegna il gioco invece che il menui
void MenuMgr::drawGame() {
	resetShowing();				//nessun menu disegnato

	winMgr.paused = false;		//avvia disegno e gestione input gioco

	
	sndMgr->playBackgroundMusic("Data/audio/monkeyislandsecretsintro.mp3");	//avvia musica sottofondo

}

//gestisce i tasti premuti dall'utente
void MenuMgr::manageKeyPressed(bool *keys, int size, float deltaT) {

	//sto disegnando game, no menu
	if (!showingMainMenu && !showingGAMEOVER && !showingWin && !showingHelp)
		return;

	timeSinceLastKeypress += deltaT;

	//sono nel Main menu
	if (showingMainMenu) {
			
		
		//posso premere un tasto ogni 100ms, evito di "saltare" elem. menu perchè selez. cambia troppo velocemente.
		if (timeSinceLastKeypress >= 0.100) {
			timeSinceLastKeypress = 0;	//azzero contatore

			if (keys[VK_UP]) {
				sndMgr->play("KEY");
				//selez. elem. sopra, se primo non va + su di così
				if (selectedMenuItem == 1 || selectedMenuItem == 2)
					selectedMenuItem--;

			}
			else if (keys[VK_DOWN]) {
				sndMgr->play("KEY");
				if (selectedMenuItem == 0 || selectedMenuItem == 1)
					selectedMenuItem++;

			}
			else if (keys[VK_RETURN]) {
				sndMgr->play("MENUKEY");
				if (selectedMenuItem == 0)
					drawGame();
				else if (selectedMenuItem == 1)
					drawHelp();
				else if (selectedMenuItem == 2)
					winMgr.quit();

			}

		} //timeSinceLastKeyPRess > ...

	}
	else if (showingGAMEOVER) {
		if (timeSinceLastKeypress >= 0.400 && keys[VK_RETURN] ) {
			//qls tasto si prema si finisce nel main menu
			//showingGAMEOVER = false;
			resetShowing();
			showingMainMenu = true;
			timeSinceLastKeypress = 0;
			sndMgr->play("KEY");
		}
	}
	else if (showingWin) {
		if (timeSinceLastKeypress >= 0.400 && keys[VK_RETURN] ) {
			//qls tasto si prema si finisce nel main menu
			//showingWin = false;
			resetShowing();
			showingMainMenu = true;
			timeSinceLastKeypress = 0;
			sndMgr->play("KEY");
		}
	}
	else if (showingHelp) {

		//evita di prendere 2 invio in successione
		if (timeSinceLastKeypress >= 0.400 && keys[VK_RETURN] ) {
			timeSinceLastKeypress = 0;
			//showingHelp = false;
			resetShowing();
			showingMainMenu = true;
			timeSinceLastKeypress = 0;
			sndMgr->play("KEY");
		}

	}


}


//gestisce movim. mouse e tasto sx
void MenuMgr::manageMouseMovedOrPressed(POINTS p) {

	//...

}

