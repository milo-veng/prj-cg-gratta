#pragma once

#include <vector>
#include <string>
using namespace std;

#include "Level.h"




class LevelsMgr
{

	Level *level;
	int levelNum;


public:
	LevelsMgr();
	~LevelsMgr();

	int getActiveLevelNum() { return levelNum; }

	//dealloca livello attuale e carica il livello num
	bool loadLevel(int num);


	//nomi dei file contenenti le mappe 3d e le BB per ogni livello
	vector<string> terrainFiles;
	vector<string> bbFiles;
	

	string getLevelTerrainFilename(int lNum) { return terrainFiles.at(lNum); }
	string getLevelBBFilename(int lNum) { return bbFiles.at(lNum); }

};

