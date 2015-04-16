
#include <Windows.h>
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>												// Header File For The Glaux Library


#include "MilkshapeModel.h"
#include "BoundingBox2D.h"

#pragma once



class Pickable3DObject : public MilkshapeModel
{
public:
	enum Pickable3DObjectType {
		GEM,
		AKUAKU
	};


private:
	int ID;					//num. UNIVOCO che identifica l'oggetto(corrisp. alla posiz. nel vett. gems\masks in PickableObjectManager

	BoundingBox2D *bb;
	BoundingBox2D randomPosLimits;		//contiene il rettangolo dentro al quale può essere posiz. a caso da setRandomPosition()

	int points;			//# di punti che il giocatore riceve quando ci passa sopra
	//i dati del modello e le funz. per il disegno sono ereditate da Model.h

	float rotationSpeed;
	float angle;			//angolo di rotazione
	bool active;			//se true viene disegnato ed è collidibile

	Pickable3DObjectType objType;

public:

	Pickable3DObject(int id, Pickable3DObjectType type, int points = 10);
	~Pickable3DObject();


	BoundingBox2D *getBoundingBox() { return bb; }


	void setActive(bool a) { this->active = a; }

	//imposta posizione oggetto e bounding box, specifica anche dimensioni bounding box
	void setPosition(float x, float y, float z);

	//loadModelData è ereditato da MilkshapeModel

	
	//riposiziona l'oggetto in un punto qualsiasi dentro al rettangolo
	//e ricalcola la bounding box
	//definito da limit sul piano X-Z
	void setRandomPosition(BoundingBox2D limit);


	//se sta collidendo con other ritorna true
	bool isCollidingWith(BoundingBox2D other);


	//ridefinisco la funzione draw(), si occupa di disegnare e ruotare l'oggetto 3d
	virtual void draw(float deltaT);


	//disegna la bounding box dell'oggetto(all'altezza predefinita di 0.0f)
	void drawBoundingBoxes();
};

