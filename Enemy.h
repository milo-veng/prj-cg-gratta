#pragma once
#include "MilkshapeModel.h"
#include "MyFPSCamera.h"
#include "BoundingBox2D.h"

extern MyFPSCamera camera;

class Enemy :
	public MilkshapeModel
{

public:
	float speed,lx,lz,xe,ze;
	BoundingBox2D bound;
	bool active;	//se true viene disegnato e gestite le collisioni

public:
	Enemy();
	void setAngle(double deltaT);
	bool isColliding(BoundingBox2D playerBox);
	void draw();		//ridefinisco
	void drawBoundingBoxes();

	//usato per riposizionarlo al suo posto riattivato, usato quando si ricarica liv. dopo la sconfitta
	void enableAndResetPos();
};

