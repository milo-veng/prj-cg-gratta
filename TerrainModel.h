#pragma once

#include <Windows.h>
#include "MilkshapeModel.h"


class TerrainModel
{
public:

	//casella libera o occupata
	enum COLMATRIXELEM {
		FREE,
		OCCUPIED
	};




	bool terrainLoaded;

	Model *pModel;
	bool **collisionMatrix; 



	//carica il modello .ms3d
	void loadTerrainModel();

	//disegna il terreno
	void draw() { pModel->draw(); }


	//calcola i due punti piu distanti sull'asse x e ritorna la loro distanza
	float getMapExtensionXAxis();

	//calcola i due punti piu distanti sull'asse z e ritorna la loro distanza
	float getMapExtensionZAxis();




	//genera la matrice delle collisioni -> da chiamare all'inizio dopo loadTerrainModel()
	void generateCollisionMatrix( const int size );
	


	TerrainModel(void);
	~TerrainModel(void);
};

