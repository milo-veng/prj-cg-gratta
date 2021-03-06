


#include <Windows.h>
#include <string>
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>												// Header File For The Glaux Library
using namespace std;

#pragma once

#include "MilkshapeModel.h"
#include "BoundingBox2D.h"


/*
	Gestisce il caricamento, il disegno e le collisioni del mondo 3D e degli oggetti statici stanti su di esso
*/
class TerrainModel
{

public:

	//casella libera o occupata
	/*enum COLMATRIXELEM {
		FREE,
		OCCUPIED
	};*/




	bool terrainLoaded;
	bool boundingBoxesLoaded;

	Model *pModel;		//modello ms3d della mappa
	BoundingBox2D mapLimits;		//limiti della mappa, il giocatore collide con essi!
	
	Model *pBoundingBoxes; //modello contenente le bounding boxes degli elementi della mappa(alberi, rocce,...)
	BoundingBox2D *bb;		//struttura per gestire le Bounding Box come rettangolo 2D sul piano  x-z
	int boundingBoxNum;		//numero di struttue Bounding Box in bb

	//bool **collisionMatrix; 

	//distanza max. tra due punti su asse X e Z
	//chiamare calcMapExtensionXAxis, calcMapExtensionXAxis per inizializzare
	float terrainSizeX;
	float terrainSizeZ;




	//carica il modello .ms3d
	void loadTerrainModel( string ms3dFilename );

	//carica le bounding boxes
	void loadBoundingBoxes( string ms3dFilename );

	//disegna il terreno
	void draw() { pModel->draw(); }

	//funzione per DEBUG
	void drawBoundingBoxes();


	//se player sta collidendo con una bounding box ritorna la bb corrispondente
	//null altrimenti
	BoundingBox2D isCollidingWith( BoundingBox2D playerBox );
	bool isInsideMapLimits( BoundingBox2D playerBox );
	
	//gestione collisioni su piano
	//ritorna true se il rettangolo passato come argomento � dentro una delle facce piatte



	//calcola i due punti piu distanti sull'asse x e ritorna la loro distanza
	//float calcMapExtensionXAxis();

	//calcola i due punti piu distanti sull'asse z e ritorna la loro distanza
	//float calcMapExtensionZAxis();

	//genera la matrice delle collisioni -> da chiamare all'inizio dopo loadTerrainModel()
	//void generateCollisionMatrix( const int size );
	


	TerrainModel(BoundingBox2D map_limits);
	~TerrainModel(void);
};

