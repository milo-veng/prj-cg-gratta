#include "Level.h"


Level::Level(vector<float> posX1, vector<float> posZ1, BoundingBox2D mapLimits)
{
	levelLoaded = false;

	//questi valori di default sono quelli del livello 1
	GLfloat _ambient[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat _diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat _lightPosition[4] = { 0.0f, 30.0, 0.0f, 1.0f };
	GLfloat _fogColor[4] = { 0.028f, 0.39f, 0.7f, 1.0f  };

	for (int i = 0; i < 4; i++) {
		ambient[i] = _ambient[i];
		diffuse[i] = _diffuse[i];
		lightPosition[i] = _lightPosition[i];
		fogColor[i] = _fogColor[i];
	}

	fogDensity = 0.0025f;
	limits = BoundingBox2D(-140.0f, -32.0f, 268.0f, 168.0f);		//limits livello 1
	gemNum = 30;
	masksNum = 3;


	//posiz. masks per liv1
#if (_MSC_VER >= 1800)	
	vector<float> posX = { 92.8f, -136.0f, 20.0f };
	vector<float> posZ = { -127.9f, -101.8f, 20.0f };
#else
	vector<float> posX; posX.push_back(92.8f); posX.push_back(-136.0f); posX.push_back(20.0f);
	vector<float> posZ; posZ.push_back(-127.9f); posZ.push_back(-101.8f); posZ.push_back(20.0f);
#endif
	//####
	masksPosX = posX;
	masksPosZ = posZ;



	terrain = new TerrainModel(mapLimits);
	objMgr = new PickableObjectsManager();

	fogEnabled = true;
	wireframeOnly = false;
	drawBoundingBoxes = false;

}


Level::~Level()
{
}




//iniz. Terrain, skybox, PickableObjsMgr, Suoni
void Level::loadLevel(string terrainFilename, string terrainBBFilename) {
	/* 1 -CARICAMENTO OGGETTI 3D */

	//carica la mappa del mondo
	logFile << "Caricamento file del mondo..." << endl;
	//terrain->loadTerrainModel("data/lowPolyLandscape.ms3d");
	terrain->loadTerrainModel(terrainFilename);

	logFile << "Caricamento file bounding box del mondo..." << endl;
	//terrain->loadBoundingBoxes("data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");
	terrain->loadBoundingBoxes(terrainBBFilename);

	logFile << "Caricamento e posizionamento gemme e maschere...";

	//Mette sulla mappa le gemme(pos. random) e le maschere(pos. predefinite)
	objMgr = new PickableObjectsManager();
	objMgr->placeGems(limits, terrain, gemNum);
	objMgr->placeMasks(limits, terrain, masksPosX, masksPosZ);

	
	//fa partire musica di sfondo
	if (sndMgr != NULL)
		sndMgr->playBackgroundMusic(backgroundMusicFile);


	levelLoaded = true;
}



//chiamato in initGL
void Level::reloadTextures() {
	if (!levelLoaded)
		return;

	terrain->pModel->reloadTextures();

	objMgr->reloadTextures();


	//init. skybox
	//carico texture skydome
	skydomeTexture = LoadGLTexture("data/skydome.bmp");
	skydome = gluNewQuadric();
	gluQuadricTexture(skydome, GLU_TRUE);


}

//dealloca elementi caricati in memoria da loadLevel()
void Level::unloadLevel() {
	if (!levelLoaded)
		return;
	
	//sndMgr->stopBackgroundMusic();
	gluDeleteQuadric(skydome);


	delete terrain;
	delete objMgr;

	masksPosX.clear();
	masksPosZ.clear();

	levelLoaded = false;

}



//disegna tutti i componenti del livello
void Level::drawLevel(float deltaT) {
	if (!levelLoaded)
		return;

	if (wireframeOnly) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	//disegno SKYDOME
	if (fogEnabled) { glDisable(GL_FOG); }				//se la nebbia è attiva la disabilita in modo da non coprire la skybox
	glBindTexture(GL_TEXTURE_2D, skydomeTexture);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();											//salva lo stato corrente prima di fare la rotazione
	gluQuadricOrientation(skydome, GLU_INSIDE);			//inverte le normali, così posso usare l'illuminazione dentro alla sfera
	//glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );					//ruota la sfera da creare
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);					//ruota la sfera da creare
	int raggio = 800, slices = 32;
	gluSphere(skydome, raggio, slices, 32);
	glPopMatrix();											//rimette a posto dopo la rotazione
	if (fogEnabled) { glEnable(GL_FOG); }
	

	objMgr->drawAll(deltaT);

	if (drawBoundingBoxes) {
		//disegna bb del oggetto, lo faccio qui a parte e non tutti insieme in fondo per problemi di traslazione
		//aku->drawBoundingBoxes();
		objMgr->drawAllBoundingBoxes();
	}

	//disegno modelli 3D
	terrain->draw();

	//disegna le bounding box del terrain e della camera
	if (drawBoundingBoxes) {
		terrain->drawBoundingBoxes();
	}


}