#include "LevelsMgr.h"
#include "MyFPSCamera.h"


LevelsMgr::LevelsMgr()
{
	level = NULL;
	levelNum = 0;	//0 = nessun livello attivato


	//impostazioni per ciascun livello:
	//terrainFiles.push_back("Data/lowPolyLandscape.ms3d");	//mappa liv.1
	//terrainFiles.push_back("Data/floatingIslandSnowRoseTreems3dBig.ms3d");	//mappa liv.2 lowPolyFloatingIslandPineta.ms3d
	terrainFiles.push_back("Data/level1Complete.ms3d");	//mappa liv.1
	terrainFiles.push_back("Data/lowPolyFloatingIslandPineta.ms3d");

	bbFiles.push_back("Data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");	//bb liv.1
	bbFiles.push_back("Data/lowpolyLandscapeOnly2DBBVeryFinal.ms3d");	//bb liv.2

	backgroundMusicFiles.push_back("Data/audio/monkeyislandsecretsintro.mp3");	//bkgnd music liv.1
	backgroundMusicFiles.push_back("Data/audio/monkeyislandsecretsintro.mp3");	//bkgnd music liv.2

	gemNum.push_back(30);	//# gemme liv.1
	gemNum.push_back(30);	//# gemme liv.2


	//limiti area in cui spawnano random le gemme
	BoundingBox2D limit1(-142.0f, -32.0f, 262.0f, 164.0f);
	BoundingBox2D limit2(-142.0f, -32.0f, 262.0f, 164.0f);
	limits.push_back(limit1);
	limits.push_back(limit2);


	//posizioni delle maschere per ciascun livello
	vector<float> posX_level1, posX_level2;
	vector<float> posZ_level1, posZ_level2;

#if (_MSC_VER >= 1800)	
	posX_level1 = { 92.8f, -136.0f, 20.0f };
	posZ_level1 = { -127.9f, -101.8f, 20.0f };

	posX_level2 = { 92.8f, -136.0f, 20.0f };
	posZ_level2 = { -127.9f, -101.8f, 20.0f };
#else
	posX_level1.push_back(92.8f); posX_level1.push_back(-136.0f); posX_level1.push_back(20.0f);
	posZ_level1.push_back(-127.9f); posZ_level1.push_back(-101.8f); posZ_level1.push_back(20.0f);
	
	posX_level2.push_back(92.8f); posX_level2.push_back(-136.0f); posX_level2.push_back(20.0f);
	posZ_level2.push_back(-127.9f); posZ_level2.push_back(-101.8f); posZ_level2.push_back(20.0f);
#endif

	//inserisco le posizioni delle singole maschere nel vettore principale
	posX.push_back(posX_level1); posX.push_back(posX_level2);
	posZ.push_back(posZ_level1); posZ.push_back(posZ_level2);



	//boundaries di ciascun livello, il giocatore resta sempre dentro questo rettangolo, non può uscirne.
	BoundingBox2D l1(-140.0f, -126.0f, 260.0f, 259.0f);
	BoundingBox2D l2(-140.0f, -131.0f, 260.0f, 264.0f);
	mapLimits.push_back(l1);
	mapLimits.push_back(l2);


}


LevelsMgr::~LevelsMgr()
{
}


extern MyFPSCamera camera;

bool LevelsMgr::loadLevel(int num) {

	//fermo le collisioni con gli oggetti del mondo
	camera.collisionsEnabled = false;

	//dealloca liv. attuale
	if (level != NULL && level->isLevelLoaded() ) {

		logFile << "Deallocazione livello " << levelNum << endl;

		level->unloadLevel();
		delete level;

	}

	logFile << "Carico il livello " << num << endl;
	 
	
	level = new Level(this->posX.at(num-1),this->posZ.at(num-1), mapLimits.at(num-1));


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


	//riattivo le collisioni con gli oggetti del mondo
	camera.collisionsEnabled = true;

	return true;
}


