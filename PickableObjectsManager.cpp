#include "PickableObjectsManager.h"

extern ofstream logFile;

PickableObjectsManager::PickableObjectsManager()
{
	RANDOM_PLACING_MAX_ITERATIONS = 100;
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
		bool done = false; int iterations = 0;
		while (!done) {

			obj->setRandomPosition(limits);

			if (isGoodPosition(*obj->getBoundingBox(), limits, terrain))
				done = true;

			iterations++;
			if (iterations > RANDOM_PLACING_MAX_ITERATIONS) {
				//raggiunto il limite considero la posiz. buona 
				MessageBox(NULL, "Raggiunto limite max. iterazioni algo. posizionam. random gemme/maschere!", "Attenzione!", MB_OK);
				break;
			}
		}
		logFile << "Piazzata una gemma in (x,z)=" << obj->getBoundingBox()->x << ", " << obj->getBoundingBox()->z << endl;

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



//ritorna true se la gemma non è troppo vicina ad un altra e se non sta collidendo con le BB del terrain(alberi, ecc...)
//obj è l'oggetto da piazzare in pos. random, limits è l'area dentro la quale metterle, terrain sono le BB della mappa
bool PickableObjectsManager::isGoodPosition(BoundingBox2D obj, BoundingBox2D limits, TerrainModel *terrain) {
	
	//sta fuori dall'area all'interno della quale l'obj deve stare(limits)?
	if (!( obj.x < limits.x+limits.w &&
			obj.x + obj.w > limits.x &&
			obj.z < limits.z + limits.h &&
			obj.z + obj.h > limits.z) ) {

		return false;	//fuori dall'area limite, non va bene!
	
	}


	//collide con qualche elemento del Terrain( alberi, rocce, ...)
	for (int i = 0; i < terrain->boundingBoxNum; i++) {

		BoundingBox2D b = terrain->bb[i];

		if (obj.x < b.x + b.w &&
			obj.x + obj.w > b.x &&
			obj.z + b.z + b.h &&
			obj.z + obj.h > b.z) {
			//vi è una collisione!
			return false;
		}

	}


	return true;
}



//ricarica le tex. di tutti i 3d objs, chiamato da GLinit
//
void PickableObjectsManager::reloadTextures() {

	//gemme
	for (int i = 0; i < gems.size(); i++) {
		gems.at(i)->reloadTextures();
	}

	//makuku
	for (int i = 0; i < masks.size(); i++) {
		masks.at(i)->reloadTextures();
	}

}


//disegna tutti i pickable 3d objs.
void PickableObjectsManager::drawAll(float deltaT) {

	//gemme
	for (int i = 0; i < gems.size(); i++) {
		gems.at(i)->draw(deltaT);
	}

	//makuku
	for (int i = 0; i < masks.size(); i++) {
		masks.at(i)->draw(deltaT);
	}

}


void PickableObjectsManager::drawAllBoundingBoxes() {


	//gemme
	for (int i = 0; i < gems.size(); i++) {
		gems.at(i)->drawBoundingBoxes();
	}

	//makuku
	for (int i = 0; i < masks.size(); i++) {
		masks.at(i)->drawBoundingBoxes();
	}

}
