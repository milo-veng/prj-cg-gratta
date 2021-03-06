#include <new>
#include <cmath>
#include <string>
#include <fstream>
#include "TerrainModel.h"

using namespace std;


extern ofstream logFile;


TerrainModel::TerrainModel(BoundingBox2D map_limits)
{
	
	terrainLoaded = false;
	boundingBoxesLoaded = false;
	pModel = new MilkshapeModel();
	pBoundingBoxes = new MilkshapeModel();

	bb = NULL;

	mapLimits = map_limits;
	
}


TerrainModel::~TerrainModel(void)
{

}



//carica il modello .ms3d
void TerrainModel::loadTerrainModel(string ms3dFilename) {

		if ( pModel->loadModelData( ms3dFilename.c_str() ) == false )		// Loads The Model And Checks For Errors
		{
			MessageBox( NULL, "WinMain(...): impossibile caricare il modello ms3d!", "Error", MB_OK | MB_ICONERROR );
			logFile << "TerrainModel::loadTerrainModel(): impossibile caricare il modello ms3d!" << endl;
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
		boundingBoxNum = nVertices / 4;						//avr� nVertices/4 rettangoli
		

		//alloca memoria per i rettangoli
		try {

			bb = new BoundingBox2D[ boundingBoxNum ];

		} catch( bad_alloc ) {
			MessageBox( NULL, "Impossibile allocare memoria per bounding box 2d!", "Allocation Error", MB_OK|MB_ICONERROR );
			return;
		}


		//genero le boundinBox2D -> cio� iniz. i valori (x,y,w,h) di ciascun rettangolo
		//logFile << "BoundingBoxes (x,z,w,h):" << endl;
		for( int i = 0, bbi = 0; i < nVertices; i+=4,bbi++ ) {
			
			//logFile << "BB #" << i/4 << ": ";
			
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

	//logFile << endl << endl << "TerrainModel:isCollidingWith(playerBox)" << endl;
	
	//scorre tutti i boundingBox della mappa 
	for( int i = 0;  i <  boundingBoxNum; i++ ) {
		BoundingBox2D tmpbb = bb[i];
		
		//logFile << "Ricerco collisione tra (" << tmpbb.x << "," << tmpbb.z << "," << tmpbb.w << "," << tmpbb.h << ") e (" << playerBox.x << "," << playerBox.z << "," << playerBox.w << "," << playerBox.h << ")"<< endl;

		if( tmpbb.x < playerBox.x + playerBox.w &&
			tmpbb.x + tmpbb.w > playerBox.x &&
			tmpbb.z < playerBox.z + playerBox.h &&
			tmpbb.h + tmpbb.z > playerBox.z ) 
		{
			//collisione avvenuta!
			collider = tmpbb;
			//logFile << "TerrainModel::isCollidingWith(camera): collisione con (x,z,w,h)=" << collider.x << "," << collider.z << "," << collider.w << "," << collider.h << endl;
			break;
		} 

	} 


	//no collision
	return collider;
}


//ritorna true se il giocatore si trova dentro ai limiti della mappa
bool TerrainModel::isInsideMapLimits( BoundingBox2D playerBox ) {
	BoundingBox2D tmpbb = mapLimits;

	if( tmpbb.x < playerBox.x + playerBox.w &&
			tmpbb.x + tmpbb.w > playerBox.x &&
			tmpbb.z < playerBox.z + playerBox.h &&
			tmpbb.h + tmpbb.z > playerBox.z ) 
		{
			return true;
		} 

	return false;

}


void TerrainModel::drawBoundingBoxes() {

	//bb degli elementi collidibili della mappa(alberi, rocce, ...
	pBoundingBoxes->draw();

	//bb map boundaries
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBegin(GL_QUADS);
	glVertex3f( mapLimits.x, 1.0f, mapLimits.z);
	glVertex3f( mapLimits.x, 1.0f, mapLimits.z + mapLimits.h);
	glVertex3f( mapLimits.x + mapLimits.w, 1.0f, mapLimits.z + mapLimits.h);
	glVertex3f( mapLimits.x + mapLimits.w, 1.0f, mapLimits.z);
	glEnd();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
}