#include <new>
#include <cmath>
#include "TerrainModel.h"

using namespace std;


TerrainModel::TerrainModel()
{
	
	terrainLoaded = false;
	pModel = new MilkshapeModel();
}


TerrainModel::~TerrainModel(void)
{

}



//carica il modello .ms3d
void TerrainModel::loadTerrainModel() {

		if ( pModel->loadModelData( "data/lowpolyLandscape.ms3d" ) == false )		// Loads The Model And Checks For Errors
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




//calcola i due punti piu distanti sull'asse x(punti estremi) e ritorna la loro distanza
float TerrainModel::getMapExtensionXAxis() {

	float d = 0.0;

	//DA FARE

	return d;
}

//calcola i due punti piu distanti sull'asse z e ritorna la loro distanza
float TerrainModel::getMapExtensionZAxis() {
	float d = 0.0;

	//DA FARE

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



