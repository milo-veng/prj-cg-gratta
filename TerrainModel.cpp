#include <new>
#include <cmath>
#include <string>
#include <fstream>
#include "TerrainModel.h"

using namespace std;


extern ofstream logFile;


TerrainModel::TerrainModel()
{
	
	terrainLoaded = false;
	boundingBoxesLoaded = false;
	pModel = new MilkshapeModel();
	pBoundingBoxes = new MilkshapeModel();

	bb = NULL;
}


TerrainModel::~TerrainModel(void)
{

}



//carica il modello .ms3d
void TerrainModel::loadTerrainModel(string ms3dFilename) {

		if ( pModel->loadModelData( ms3dFilename.c_str() ) == false )		// Loads The Model And Checks For Errors
		{
			MessageBox( NULL, "WinMain(...): impossibile caricare il modello ms3d!", "Error", MB_OK | MB_ICONERROR );
			return;													// If Model Didn't Load Quit
		}
		else {
			terrainLoaded = true;
			//traslo la mappa nell'origine
			pModel->translateModel( 0.0, 0.0, 0.0 );
		}

}


//carica il modello .ms3d contenente SOLO le bounding box
void TerrainModel::loadBoundingBoxes( string ms3dFilename ) {

		if ( pBoundingBoxes->loadModelData( ms3dFilename.c_str() ) == false )		// Loads The Model And Checks For Errors
		{
			MessageBox( NULL, "WinMain(...): impossibile caricare le bounding boxes!", "Error", MB_OK | MB_ICONERROR );
			return;													// If Model Didn't Load Quit
		}
		
	
		
		boundingBoxesLoaded = true;

		//traslo la mappa nell'origine
		pBoundingBoxes->translateModel( 0.0, 0.0, 0.0 );

		int nVertices = pBoundingBoxes->getNumVertices();
		boundingBoxNum = nVertices / 4;						//avrò nVertices/4 rettangoli
		

		//alloca memoria per i rettangoli
		try {

			bb = new BoundingBox2D[ boundingBoxNum ];

		} catch( bad_alloc ) {
			MessageBox( NULL, "Impossibile allocare memoria per bounding box 2d!", "Allocation Error", MB_OK|MB_ICONERROR );
			return;
		}


		//genero le boundinBox2D -> cioè iniz. i valori (x,y,w,h) di ciascun rettangolo
		logFile << "BoundingBoxes (x,z,w,h):" << endl;
		for( int i = 0, bbi = 0; i < nVertices; i+=4,bbi++ ) {
			
			logFile << "BB #" << i/4 << ": ";
			
			Model::Vertex v[4];
			v[0] = pBoundingBoxes->getVertexByIndex(i);
			v[1] = pBoundingBoxes->getVertexByIndex(i+1);
			v[2] = pBoundingBoxes->getVertexByIndex(i+2);
			v[3] = pBoundingBoxes->getVertexByIndex(i+3);

			bb[bbi].createBoundingBox( v );

			//logFile << " TerrainModel::loadBoundingBoxes(): ho salvato la BB (" << bb[bbi].x << "," << bb[bbi].z << "," << bb[bbi].w << "," << bb[bbi].h << ")" << endl;
		}

		logFile << "Generate " << boundingBoxNum << " boundingBox2D" << endl;
		


}


//se player sta collidendo con una bounding box ritorna la bb corrispondente
//null altrimenti
//N.B: in caso di collisioni multiple, ritorna SOLO la prima!
BoundingBox2D TerrainModel::isCollidingWith( BoundingBox2D playerBox ) {
	
	BoundingBox2D collider;	//iniz. coi valori nulli => isNull() == true adesso

	//se arg. nullo ritorna subito
	if( playerBox.isNull() ) 
		return collider;

	logFile << endl << endl << "TerrainModel:isCollidingWith(playerBox)" << endl;
	
	//scorre tutti i boundingBox della mappa 
	for( int i = 0;  i <  boundingBoxNum; i++ ) {
		BoundingBox2D tmpbb = bb[i];
		
		logFile << "Ricerco collisione tra (" << tmpbb.x << "," << tmpbb.z << "," << tmpbb.w << "," << tmpbb.h << ") e (" << playerBox.x << "," << playerBox.z << "," << playerBox.w << "," << playerBox.h << ")"<< endl;

		if( tmpbb.x < playerBox.x + playerBox.w &&
			tmpbb.x + tmpbb.w > playerBox.x &&
			tmpbb.z < playerBox.z + playerBox.h &&
			tmpbb.h + tmpbb.z > playerBox.z ) 
		{
			//collisione avvenuta!
			collider = tmpbb;
			logFile << "Collisione con (x,z,w,h)=" << collider.x << "," << collider.z << "," << collider.w << "," << collider.h << endl;
			break;
		} 

	} 


	//no collision
	return collider;
}


/*
//BOZZE FUNZIONI PER COLLISION MATRIX

//calcola i due punti piu distanti sull'asse x(punti estremi) e ritorna la loro distanza
float TerrainModel::calcMapExtensionXAxis() {

	float d = 0.0;
	float min = 0.0, max = 0.0;

	//DA FARE
	const Model::Vertex *v = pModel->getVerticesArray();

	for( int i = 0; i < pModel->getNumVertices(); i++ ) {
		float x = v[i].m_location[0];
		
		if( x < min ) {
			min = x;
			continue;
		}

		if( x > max ) {
			max = x;
			continue;
		}
	}

	d = max - min;

	return d;
}

//calcola i due punti piu distanti sull'asse z e ritorna la loro distanza
float TerrainModel::calcMapExtensionZAxis() {
	float d = 0.0;
	float min = 0.0, max = 0.0;

	//DA FARE
	const Model::Vertex *v = pModel->getVerticesArray();

	for( int i = 0; i < pModel->getNumVertices(); i++ ) {
		float z = v[i].m_location[0];
		
		if( z < min ) {
			min = z;
			continue;
		}

		if( z > max ) {
			max = z;
			continue;
		}
	}

	d = max - min;

	return d;
}


//size -> risoluzione delle collisioni
void TerrainModel::generateCollisionMatrix(const int size) {

	//alloca matrice
	try {
		collisionMatrix = new bool*[size];
		for( int i = 0; i < size; i++ ) {
			collisionMatrix[i] = new bool[size];
		}
	} catch (std::bad_alloc& ba) {
		MessageBox( NULL, ba.what(), "Errore allocazione memoria collisionMatrix", MB_ICONERROR|MB_OK);
	}


	//inizializzo gli elem. a FREE
	for( int i = 0; i < size; i++ ) 
		for( int j = 0; j < size; j++ ) 
			collisionMatrix[i][j] = COLMATRIXELEM::FREE;




	//scandisco i vertici e guardo se sono sopraelevati rispetto al terreno
	//in quelli sopraelevati segno sulla matrice quali sono occupati
	for( int i = 0; i < pModel->getNumVertices(); i++ ) {
	
		Model::Vertex v = pModel->getVertexByIndex( i );

		// è sopraelevato
		if( v.m_location[1] > 0 ) {

			//devo prima aver calcolato la larghezza della mappa su asse x e dividerlo per size
			//per ottenere la grandezza di una casella, idem per l'asse z
			
			//...

		}

	}


}



*/