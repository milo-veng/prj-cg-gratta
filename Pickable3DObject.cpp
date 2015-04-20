#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cmath>
using namespace std;

#include "randomNum.h"
#include "Pickable3DObject.h"


#define PI_GRECO 3.1415926535897

extern ofstream logFile;


//costruisce un Pickable3DObj del tipo specificato(GEM,AKU) e stabilisce il punteggio
//assegnato al giocatore quando lo raccoglie, valore di default = 10(gemma standard)
//id è un num. UNIVOCO che identifica l'oggetto
Pickable3DObject::Pickable3DObject(int id, Pickable3DObjectType type, int points )
{
	bb = new BoundingBox2D(0.0f, 0.0f, 0.0f, 0.0f);
	bb->x = 0.0f;
	bb->z = 0.0f;
	
	objType = type;

	//imposta la dimensione della bounding box in base al tipo di oggetto rappresentato
	if (type == Pickable3DObjectType::GEM) {
		//OK
		bb->w = 2.8f;
		bb->h = 2.8f;
	}
	else if (type == Pickable3DObjectType::AKUAKU) {
		//da verificare!!!!!!!!!!!!!!!
		bb->w = 1.0f;
		bb->h = 2.0f;
	}



	float rotationSpeed = 0.000001f;

	bool active = true;

	angle = 0.0;
}


Pickable3DObject::~Pickable3DObject()
{
}


//imposta posizione oggetto e bounding box, specifica anche dimensioni bounding box
void Pickable3DObject::setPosition(float x, float y, float z) {

	//sposto modello 3D
	translateModel(x, y, z);

	//aggiorno BB, l'oggetto sta al centro della bounding box
	//w e h di bb dipendono dal Pickable3DObjectType specificato e sono impostati auto. dal costruttore
	bb->x = x - bb->w / 2;
	bb->z = z - bb->h / 2;

}


//riposiziona l'oggetto in un punto qualsiasi dentro al rettangolo
//e ricalcola la bounding box
//definito da limit sul piano X-Z
//N.B: non controlla se due oggetti siano troppo vicini o sovrapposti!
void Pickable3DObject::setRandomPosition(BoundingBox2D limit) {

	randomPosLimits = limit;


	
	//genera due valori casuali contenuti dentro i limiti della bounding box limits
	//float randX = rand() % (int)(randomPosLimits.w) + randomPosLimits.x;
	//float randZ = rand() % (int)(randomPosLimits.h) + randomPosLimits.z;
	float randX = getRandomFloat(randomPosLimits.x, randomPosLimits.x + randomPosLimits.w);
	float randZ = getRandomFloat(randomPosLimits.z, randomPosLimits.z + randomPosLimits.h);
	

	//sposto il modello+
	setPosition(randX, 0.0f, randZ);

}


//true se l'oggetto passato come argomento sta collidendo con questo
bool Pickable3DObject::isCollidingWith(BoundingBox2D other) {
	BoundingBox2D collider;	//iniz. coi valori nulli => isNull() == true adesso

	//se arg. nullo ritorna subito
	if (other.isNull())
		return false;

	
	if (bb->x < other.x + other.w &&
		bb->x + bb->w > other.x &&
		bb->z < other.z + other.h &&
		bb->h + bb->z > other.z)
	{
		//collisione avvenuta!
		logFile << "Pickable3DObject::isCollidingWith(other(): collisione con (x,z,w,h)=" << other.x << "," << other.z << "," << other.w << "," << other.h << endl;
		return true;
	}


	return false;
}


//ridefinisco la funzione draw() di Model.h
void Pickable3DObject::draw(float deltaT) {
	
	if (!active)
		return;


	glPushMatrix();		//### lo aggiungo per colpa della chiama a glTranslatef in questa funzione
	//che mi sposta tutto il sist. di rif., con il popMatrix faccio tornare tutto come era prima della chiamata

	GLboolean texEnabled = glIsEnabled(GL_TEXTURE_2D);

	// Draw by group
	for (int i = 0; i < m_numMeshes; i++)
	{

		//applico materiali + texture
		int materialIndex = m_pMeshes[i].m_materialIndex;
		if (materialIndex >= 0)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_pMaterials[materialIndex].m_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_pMaterials[materialIndex].m_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_pMaterials[materialIndex].m_specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, m_pMaterials[materialIndex].m_emissive);
			glMaterialf(GL_FRONT, GL_SHININESS, m_pMaterials[materialIndex].m_shininess);

			if (m_pMaterials[materialIndex].m_texture > 0)
			{
				glBindTexture(GL_TEXTURE_2D, m_pMaterials[materialIndex].m_texture);
				glEnable(GL_TEXTURE_2D);
			}
			else
				glDisable(GL_TEXTURE_2D);
		}
		else
		{
			// Material properties?
			glDisable(GL_TEXTURE_2D);
		}


		//N.B: le funzioni vanno chiamate nell'ordine inverso a quello in cui vengono eseguite
		//glTranslatef(-xpos, -ypos, -zpos); //porto nell'origine
		float oldX = xpos; float oldY = ypos; float oldZ = zpos;

		//glTranslatef(xpos, ypos, zpos);	//3) riporto nella posiz. di partenza
		glTranslatef(oldX, oldY, oldZ);


		if (angle + rotationSpeed*deltaT >= 2 * PI_GRECO)
			angle = 0.0f;

		angle += rotationSpeed*deltaT*0.0000001f;
		glRotatef(-angle, 0.0f, 1.0f, 0.0f);  // 2) ruoto su se stesso poichè sono nell'origine

		
		glTranslatef(0.0f, 0.0f, 0.0f);			//1) trasla nell'origine

		


		//disegno triangoli
		glBegin(GL_TRIANGLES);
		{
			for (int j = 0; j < m_pMeshes[i].m_numTriangles; j++)
			{
				int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
				const Triangle* pTri = &m_pTriangles[triangleIndex];

				for (int k = 0; k < 3; k++)
				{
					int index = pTri->m_vertexIndices[k];

					glNormal3fv(pTri->m_vertexNormals[k]);
					glTexCoord2f(pTri->m_s[k], pTri->m_t[k]);
					glVertex3fv(m_pVertices[index].m_location);
				}
			}
		}
		glEnd();
	}

	if (texEnabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);


	glPopMatrix();		//####
}



void Pickable3DObject::drawBoundingBoxes() {


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//wireframe


	//aku BB
	glBegin(GL_QUADS);
	const BoundingBox2D *b = bb;

	glVertex3f(b->x, 0.0f, b->z);
	glVertex3f(b->x + b->w, 0.0f, b->z);
	glVertex3f(b->x + b->w, 0.0f, b->z + b->h);
	glVertex3f(b->x, 0.0f, b->z + b->h);
	glEnd();


	//setRandomPos limit bb

	glBegin(GL_QUADS);
	glVertex3f(randomPosLimits.x, 0.0f, randomPosLimits.z);
	glVertex3f(randomPosLimits.x + randomPosLimits.w, 0.0f, randomPosLimits.z);
	glVertex3f(randomPosLimits.x + randomPosLimits.w, 0.0f, randomPosLimits.z + randomPosLimits.h);
	glVertex3f(randomPosLimits.x, 0.0f, randomPosLimits.z + randomPosLimits.h);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //filled
}