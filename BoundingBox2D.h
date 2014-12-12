
#pragma once

#include "Model.h"

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
	void createBoundingBox( Model::Vertex vertices[4] );


	//ritorna true se la bounding è nulla, cioè non inizializzata. Altrim. ritorna false
	bool isNull() { 
		if( w == 0.0 && h == 0.0 && x == 0.0 && z == 0.0 ) return true; 
		else return false; 
	}


};

