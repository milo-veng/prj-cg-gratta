#pragma once

#include <vector>
#include <fstream>
using namespace std;

#include "BoundingBox2D.h"
#include "Pickable3DObject.h"
#include "TerrainModel.h"
#include "SoundMgr.h"
#include "PlayerStats.h"


extern SoundMgr *sndMgr;
extern PlayerStats pStats;


class PickableObjectsManager
{
	vector<Pickable3DObject*> gems;			//gemme sparse per la mappa
	vector<Pickable3DObject*> masks;			//aku aku sparsi per la mappa

	int gemsNum;							//# di gemme presenti( = a gems.size()) 

	int RANDOM_PLACING_MAX_ITERATIONS;

	//usati per evitare di piazzare nuove gemme troppo vicine all'ultima piazzata
	float oldX;
	float oldZ;

	bool gemsAndMasksPlaced;		//true quando masks e gems sono state piazzate

public:

	static int masksNum;						//# di maschere presenti( = a masks.size() )


	PickableObjectsManager();
	~PickableObjectsManager();

	bool placeGems(BoundingBox2D limits, TerrainModel *terrain, int num);				//piazza le gemme in giro per la mappa

	bool placeMasks(BoundingBox2D limits, TerrainModel *terrain, vector<float> posX, vector<float> posZ);				//idem
			

	void reloadTextures();																//ricarica tutte le texture dei modelli 3d, chiamato in GLinit()
	void drawAll(float deltaT);																		//disegna tutti i pickable obj negli array
	void drawAllBoundingBoxes();


	BoundingBox2D checkCollisions( BoundingBox2D camera );		//controlla se vi è qualche gemma/maschera che sta collidendo con il giocatore


	bool isGoodPosition(BoundingBox2D obj, BoundingBox2D limits, TerrainModel *terrain);		//ritorna true se la gemma non è troppo vicina ad un altra e se non sta collidendo con

	//enable di tutti i PickableObj3D a true, quando si ricarica lo stesso livello dopo aver perso la partita
	void enableAllGemsAndMasks();

};

