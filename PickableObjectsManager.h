#include <vector>
#include <fstream>
using namespace std;

#include "BoundingBox2D.h"
#include "Pickable3DObject.h"
#include "TerrainModel.h"

#pragma once


class PickableObjectsManager
{
	vector<Pickable3DObject*> gems;			//gemme sparse per la mappa
	vector<Pickable3DObject*> masks;			//aku aku sparsi per la mappa

	int gemsNum;							//# di gemme presenti( = a gems.size()) 
	int masksNum;							//# di maschere presenti( = a masks.size() )



public:
	PickableObjectsManager();
	~PickableObjectsManager();

	bool placeGems(BoundingBox2D limits, TerrainModel *terrain, int num);				//piazza le gemme in giro per la mappa

	bool placeMasks(BoundingBox2D limits, TerrainModel *terrain, int num);				//idem
			

	BoundingBox2D checkCollisions( BoundingBox2D camera );		//controlla se vi è qualche gemma/maschera che sta collidendo con il giocatore


	bool isGoodPosition(BoundingBox2D limits, TerrainModel *terrain);		//ritorna true se la gemma non è troppo vicina ad un altra e se non sta collidendo con

};

