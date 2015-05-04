#pragma once

#include <vector>
#include <string>
#include <fstream>
using namespace std;


#include "BoundingBox2D.h"
#include "Level.h"


extern ofstream logFile;



class LevelsMgr
{

	Level *level;
	int levelNum;	//# del livello attivo

	
public:

	static const int TOTAL_LEVEL_NUM = 2;	//# di livelli presnti nel gioco

	LevelsMgr();
	~LevelsMgr();

	Level *get() { return level; }


	int getActiveLevelNum() { return levelNum; }

	//dealloca livello attuale e carica il livello num
	bool loadLevel(int num);




	//nomi dei file contenenti le mappe 3d e le BB per ogni livello e le risp. musiche di sottofondo
	vector<string> terrainFiles;
	vector<string> bbFiles;
	vector<string> backgroundMusicFiles;
	vector<int> gemNum;
	vector<BoundingBox2D> mapLimits;	//limiti di ciascuna mappa per ciascun livello, il giocatore è confinato in questi rettangoli
	vector<BoundingBox2D> limits;		//rettangoli dentro ai quali vengono disseminate a caso le gemme
	vector<vector<float>> posX;	//posiz. delle maschere per ciascun livello asse X
	vector<vector<float>> posZ;	//"" asse Z
	

	string getLevelTerrainFilename(int lNum) { return terrainFiles.at(lNum-1); }
	string getLevelBBFilename(int lNum) { return bbFiles.at(lNum-1); }



	enum {
		LEVEL_1 = 1,
		LEVEL_2 = 2
	};

};

