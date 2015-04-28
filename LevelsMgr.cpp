#include "LevelsMgr.h"


LevelsMgr::LevelsMgr()
{
	level = NULL;
	levelNum = 0;	//0 = nessun livello attivato

	
	terrainFiles.push_back("data/lowPolyLandscape.ms3d");
	terrainFiles.push_back("data/lowPolyLandscape.ms3d");

	bbFiles.push_back("data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");
	bbFiles.push_back("data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");

}


LevelsMgr::~LevelsMgr()
{
}


bool LevelsMgr::loadLevel(int num) {

	//dealloca liv. attuale
	if (level != NULL && level->isLevelLoaded() ) {

		level->unloadLevel();
		delete level;

	}

	//carica il prossimo
	level = new Level();


	level->loadLevel(getLevelTerrainFilename(num), getLevelBBFilename(num));


	return true;
}


