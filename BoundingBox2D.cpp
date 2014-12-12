#include "BoundingBox2D.h"


//genera x,z,w,h a partire da 4 vertici in ordine casuale
//cioè collega i 4 vertici in modo da formare un quadrilatero
//usato per creare quadrilatero a partire dai vertici di una vertexlist di ms3d(che sono triangolati)
bool BoundingBox2D::createBoundingBox( Model::Vertex vertices[4] ) {

	float minX = 0.0, float maxX = 0.0;
	float minZ = 0.0, float maxZ = 0.0;


	//trovo il vertice con (x,z) minori e quello con (x,z) maggiori
	for( int i = 0; i < 4; i++ ) {
		
		float x = vertices[i].m_location[0];
		float z = vertices[i].m_location[2];


		if( x < minX )
			minX = x;
		else if( x > maxX )
			maxX = x;

		if( z < minZ )
			minZ = z;
		else if( z > maxZ )
			maxZ = z;

	}

	this->x = minX;
	this->z = minZ;
	this->w = maxX - minX;


}
