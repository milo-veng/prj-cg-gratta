#include "PickableObjectsManager.h"

extern ofstream logFile;

PickableObjectsManager::PickableObjectsManager()
{
}


PickableObjectsManager::~PickableObjectsManager()
{
}

//piazza num gemme sparse in modo random per la mappa 
//dentro alla BB limits. 
//Si occupa di allocare gli elementi del vettore gems e 
//di richiamare per ciascuno setRandomPosition()
bool PickableObjectsManager::placeGems(BoundingBox2D limits, TerrainModel *terrain, int num) {

	//alloca le gemme
	for (int i = 0; i < num; i++) {
		//gemma standard(10 punti)
		Pickable3DObject *obj = new Pickable3DObject(i, Pickable3DObject::Pickable3DObjectType::GEM);
		
		//carica modello 3d gemma
		if (!obj->loadModelData("data/gem.ms3d")) {
			logFile << "PickableObjectsManager::placeGems(): Impossibile caricare modello gemma!" << endl;
			return false;
		}

		//obj->setActive(false);		//per ora disattiva disegno e collisioni




		//piazza le gemme sulla mappa in modo random tenendo conto che non devono essere troppe vicine tra loro ne sovrapporsi ne collidere con le BB degli alberi
		//ciclo: se setRandomPos() e isGoodPosition() -> OK, altrim. ripeti
		//.########################## -.......... ‡˘#####‡‡‡‡‡‡‡‡#################
		//...






		//inserisce il puntatore nel vettore
		gems.push_back(obj);

	} //for



	


	return true;

}


//piazza num maschere sparse in modo random per la mappa 
//dentro alla BB limits. 
//Si occupa di allocare gli elementi del vettore gems e 
//di richiamare per ciascuno setRandomPosition()
bool PickableObjectsManager::placeMasks(BoundingBox2D limits, TerrainModel *terrain, int num) {



	return true;
}


//controlla per ogni gemma e per ogni maschera se sta collidendo con il player(camera)
BoundingBox2D PickableObjectsManager::checkCollisions(BoundingBox2D camera) {
	BoundingBox2D bb(0.0f, 0.0f, 0.0f, 0.0f);


	return bb;
}



//ritorna true se la gemma non Ë troppo vicina ad un altra e se non sta collidendo con le BB del terrain(alberi, ecc...)
bool PickableObjectsManager::isGoodPosition(BoundingBox2D limits, TerrainModel *terrain) {

	return true;
}
