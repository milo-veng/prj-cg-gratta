#include "Model.h"


#pragma once
class BoundingBox2D
{
public:
	

	float x, z; //vertici iniziali
	float w, h; //larghezza e altezza



	BoundingBox2D() { x = z = w = h = 0.0; }
	BoundingBox2D( float bbX, float bbZ, float bbW, float bbH )  {
		x = bbX; z = bbZ;
		w = bbW; h = bbH;
	}


	//genera x,z,w,h a partire da 4 vertici in ordine casuale
	//cioè collega i 4 vertici in modo da formare un quadrilatero
	bool createBoundingBox( Model::Vertex vertices[4] );



};

