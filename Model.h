/*
	Model.h

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/
#include <Windows.h>
#include <gl/GL.h>


extern GLuint LoadGLTexture( const char *filename );


#ifndef MODEL_H
#define MODEL_H

class Model
{
	public:
		//	Mesh
		struct Mesh
		{
			int m_materialIndex;
			int m_numTriangles;
			int *m_pTriangleIndices;
		};

		//	Material properties
		struct Material
		{
			float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
			float m_shininess;
			GLuint m_texture;
			char *m_pTextureFilename;
		};

		//	Triangle structure
		struct Triangle
		{
			float m_vertexNormals[3][3]; //NORMALI!!
			float m_s[3], m_t[3];
			int m_vertexIndices[3];
		};

		//	Vertex structure
		struct Vertex
		{
			char m_boneID;	// for skeletal animation
			float m_location[3];
		};

	public:
		/*	Constructor. */
		Model();

		/*	Destructor. */
		virtual ~Model();

		/*	
			Load the model data into the private variables. 
				filename			Model filename
		*/
		virtual bool loadModelData( const char *filename ) = 0;

		/*
			Draw the model.
		*/
		virtual void draw();

		/*
			Called if OpenGL context was lost and we need to reload textures, display lists, etc.
		*/
		void reloadTextures();


		/*
			Permette di traslare il modello nel mondo
		*/
		void translateModel( float x, float y, float z ) { xpos = x; ypos = y; zpos = z; }

	protected:
		//	Meshes used
		int m_numMeshes;
		Mesh *m_pMeshes;

		//	Materials used
		int m_numMaterials;
		Material *m_pMaterials;

		//	Triangles used
		int m_numTriangles;
		Triangle *m_pTriangles;

		//	Vertices Used
		int m_numVertices;
		Vertex *m_pVertices;


		//posizione del modello nel mondo3d
		float xpos, ypos, zpos;

public: 

	//funzioni helper
	int getNumVertices() { return m_numVertices; }

	Vertex getVertexByIndex(int i ) { return m_pVertices[i]; }

	const Vertex *getVerticesArray() { return m_pVertices; }


};

#endif // ndef MODEL_H
