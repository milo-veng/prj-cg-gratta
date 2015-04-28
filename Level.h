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
#include <random>
using namespace std;

#include "BoundingBox2D.h"
#include "TerrainModel.h"
#include "Pickable3DObject.h"
#include "PickableObjectsManager.h"
#include "Sound.h"
#include "SoundMgr.h"

extern ofstream logFile;
extern SoundMgr *sndMgr;

class Level
{
	TerrainModel *terrain;						//mondo
	
	PickableObjectsManager *objMgr;				//gemme e maschere


	GLUquadricObj *skydome;					//skydome
	GLuint skydomeTexture;

	BoundingBox2D limits;					//rettangolo nel quale si possono posiz. le gemme(random)
	int gemNum;								//# di gemme da posiz. nella gem. Area
	int masksNum;							//# di maschere da piazzare


	string terrainFile, terrainBoundingBoxesFile;//file contenenti modello 3d e Bounding box del modello
	string backgroundMusicFile;					//musica di backgrd(loop)

	bool levelLoaded;



public:

	//luci livello
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat lightPosition[4];

	//nebbia livello
	GLfloat fogDensity;
	GLfloat fogColor[4];

	//posizioni maschere
	vector<float> masksPosX;
	vector<float> masksPosZ;

	bool fogEnabled;
	bool wireframeOnly;
	bool drawBoundingBoxes;



	Level();
	~Level();


	bool isLevelLoaded() { return levelLoaded; }



	//iniz. Terrain, skybox, PickableObjsMgr, Suoni
	void loadLevel(string terrainFilename, string terrainBBFilename);

	//chiamato in initGL
	void reloadTextures();

	//disegna il livello
	void drawLevel(float deltaT);


	//dealloca elementi caricati in memoria da loadLevel()
	void unloadLevel();


	//permette di impostare la posizione (x,y,z) delle N maschere
	void setMasksPosition(vector<float> posX, vector<float> posZ) { /*masksPositions = positions; */}


	//queste funzioni vanno chiamate PRIMA di loadLevel()!!!
	void setGemAreaLimits(BoundingBox2D l) { limits = l; }	///Area rettangolo all'interno della quale verranno posiz. random le gemme
	void setGemNumber(int n) { gemNum = n; }
	void setBackgroundMusicFilename(string s) { backgroundMusicFile = s;}

};

