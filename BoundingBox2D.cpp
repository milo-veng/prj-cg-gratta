#include <fstream>
#include <limits>
using namespace std;

#include "BoundingBox2D.h"


extern ofstream logFile;


//genera x,z,w,h a partire da 4 vertici in ordine casuale
//cioè collega i 4 vertici in modo da formare un quadrilatero
//usato per creare quadrilatero a partire dai vertici di una vertexlist di ms3d(che sono triangolati)
void BoundingBox2D::createBoundingBox( Model::Vertex vertices[4] ) {

	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;

	//logFile << "BoundingBox2D::createBoundingBox(), dati i vertici: " << endl;

	//trovo il vertice con (x,z) minori e quello con (x,z) maggiori
	for( int i = 0; i < 4; i++ ) {
		
		float x = vertices[i].m_location[0];
		float z = vertices[i].m_location[2];

		//logFile << "\t(x,z)=(" << x << "," << z << ")" << endl;

		if( x < minX )
			minX = x;
		
		if( x > maxX )
			maxX = x;

		if( z < minZ )
			minZ = z;
		if( z > maxZ )
			maxZ = z;

	}

	//logFile << "\tHo trovato: minX = " << minX << ",maxX=" << maxX << "; minZ=" << minZ << ",maxZ=" << maxZ << endl;

	this->x = minX;
	this->z = minZ;
	this->w = maxX - minX;
	this->h = maxZ - minZ;

	//logFile << "\tBB generata: (x,z,w,h)=(" << this->x << "," << this->z << "," << this->w << "," << this->h << ")" << endl;

}
