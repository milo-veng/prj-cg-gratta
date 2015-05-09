#include "Enemy.h"
#include "MyFPSCamera.h"
#include "BoundingBox2D.h"
#include "PlayerStats.h"
#include "WindowMgr.h"

extern PlayerStats pStats;
extern WindowMgr winMgr;

Enemy::Enemy(void)
{
	speed = 0.4;
	lx = lz = 0.0;

	xpos = -60.0f;
	zpos = 100.0f;

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
}

bool Enemy:: isColliding(BoundingBox2D playerBox)
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


void Enemy::draw()
{
	//glPushMatrix();		//### lo aggiungo per colpa della chiama a glTranslatef in questa funzione
	//che mi sposta tutto il sist. di rif., con il popMatrix faccio tornare tutto come era prima della chiamata

	if (!active)
		return;

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


		//traslo il modello nella sua posizione attuale
		glTranslatef(xpos, ypos, zpos);


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


	//glPopMatrix();		//####
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