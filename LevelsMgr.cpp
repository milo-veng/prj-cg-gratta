#include "LevelsMgr.h"


LevelsMgr::LevelsMgr()
{
	level = NULL;
	levelNum = 0;	//0 = nessun livello attivato


	//impostazioni per ciascun livello:
	terrainFiles.push_back("Data/lowPolyLandscape.ms3d");	//mappa liv.1
	//terrainFiles.push_back("Data/floatingIslandSnowRoseTreems3dBig.ms3d");	//mappa liv.2 lowPolyFloatingIslandPineta.ms3d
	terrainFiles.push_back("lowPolyFloatingIslandPineta.ms3d");

	bbFiles.push_back("Data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");	//bb liv.1
	bbFiles.push_back("Data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");	//bb liv.2

	backgroundMusicFiles.push_back("Data/audio/monkeyislandsecretsintro.mp3");	//bkgnd music liv.1
	backgroundMusicFiles.push_back("Data/audio/monkeyislandsecretsintro.mp3");	//bkgnd music liv.2

	gemNum.push_back(30);	//# gemme liv.1
	gemNum.push_back(30);	//# gemme liv.2



	BoundingBox2D limit1(-140.0f, -32.0f, 268.0f, 168.0f);
	BoundingBox2D limit2(-140.0f, -32.0f, 268.0f, 168.0f);

	limits.push_back(limit1);
	limits.push_back(limit2);
}


LevelsMgr::~LevelsMgr()
{
}


bool LevelsMgr::loadLevel(int num) {

	//dealloca liv. attuale
	if (level != NULL && level->isLevelLoaded() ) {

		logFile << "Deallocazione livello " << levelNum << endl;

		level->unloadLevel();
		delete level;

	}

	logFile << "Carico il livello " << num << endl;
	 
	

	level = new Level();


	//impostazioni del livello
	level->setGemAreaLimits(limits.at(num - 1));
	level->setGemNumber(gemNum.at(num - 1));
	level->setBackgroundMusicFilename(backgroundMusicFiles.at(num - 1));



	//CARICA LIVELLO
	level->loadLevel(getLevelTerrainFilename(num), getLevelBBFilename(num));


	if (level->isLevelLoaded()) {
		levelNum = num;
		level->reloadTextures();
		logFile << "Livello " << levelNum << " caricato correttamente" << endl;
	}
	else
		logFile << "Errore durante il caricamento del livello " << levelNum << endl;


	return true;
}


