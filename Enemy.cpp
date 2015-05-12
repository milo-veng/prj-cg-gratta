#include <cmath>
#include "Enemy.h"
#include "MyFPSCamera.h"
#include "BoundingBox2D.h"
#include "PlayerStats.h"
#include "WindowMgr.h"
#include "SoundMgr.h"

extern PlayerStats pStats;
extern WindowMgr winMgr;
extern SoundMgr *sndMgr;

static const float PI_GRECO = 3.1415926535;

Enemy::Enemy(void)
{
	speed = 0.2;
	lx = lz = 0.0;

	xpos = -90.0f;
	zpos = 90.0f;

	bound.w = bound.h = 2.0f;
	bound.x = xpos - bound.w/2;
	bound.z = zpos - bound.h/2;

	active = true;
}

//movimento fantasma in direzione del giocatore
void Enemy:: setAngle(double deltaT)
{

	//se il gioco è in pausa non aggiorna
	if (winMgr.paused)
		return;

	//inattivo dopo che ha colliso col player
	if (!active)
		return;

	lx = camera.xpos - xpos;
	lz = camera.zpos - zpos;

	double xDelta = lx*speed*deltaT;
	double zDelta = lz*speed*deltaT;

	xpos += xDelta;
	zpos += zDelta;

	bound.x = xpos - bound.w / 2;
	bound.z = zpos - bound.h / 2;


	//se è abbastanza vicino al giocatore fa partire risata malefica
	float d = sqrt(pow((camera.xpos - xpos), 2) + pow((camera.zpos - zpos), 2));
	if (d < 25.0f) {

		sndMgr->play("MORROS", true);
	}

}

bool Enemy::isColliding(BoundingBox2D playerBox)
{
	//se il gioco è in pausa non aggiorna
	if (winMgr.paused)
		return false;

	if (!active)
		return false;

	BoundingBox2D collider;

		//logFile << "Ricerco collisione tra (" << bound.x << "," << bound.z << "," << bound.w << "," << bound.h << ") e (" << playerBox.x << "," << playerBox.z << "," << playerBox.w << "," << playerBox.h << ")"<< endl;

	if( bound.x < playerBox.x + playerBox.w &&
			bound.x + bound.w > playerBox.x &&
			bound.z < playerBox.z + playerBox.h &&
			bound.h + bound.z > playerBox.z ) 
		{
			//collisione avvenuta!
			collider = bound;
			active = false;
			pStats.enemyColliding();
			logFile << "Enemy::isColliding(camera): collisione con (x,z,w,h)=" << collider.x << "," << collider.z << "," << collider.w << "," << collider.h << endl;
			return true;
		}

		return false;

}

float angle = 0.0f; float rotationSpeed = 100.0f;
void Enemy::draw()
{

	if (!active)
		return;
		

	glTranslatef(xpos, ypos, zpos);	//posiz.
		
	billboardCheatSphericalBegin();	//ruoto nella direzione del giocatore(fa push prima)


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


	billboardEnd(); //popMatrix
	
}


void Enemy::drawBoundingBoxes() {

	//disegno filled/wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glBegin(GL_QUADS);
	glVertex3f(bound.x,1.0f,bound.z);
	glVertex3f(bound.x, 1.0f, bound.z+bound.h);
	glVertex3f(bound.x+bound.w, 1.0f, bound.z+bound.h);
	glVertex3f(bound.x, 1.0f, bound.z+bound.h);
	glEnd();




	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void Enemy::enableAndResetPos() {
	lx = lz = 0.0;

	xpos = -60.0f;
	zpos = 100.0f;

	active = true;
}


void Enemy::billboardCheatSphericalBegin() {
	
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
	    for( j=0; j<3; j++ ) {
		if ( i==j )
		    modelview[i*4+j] = 1.0;
		else
		    modelview[i*4+j] = 0.0;
	    }

	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}



void Enemy::billboardEnd() {

	// restore the previously 
	// stored modelview matrix
	glPopMatrix();
}
