/*
 *  Mesh3D.h
 *  Chess3DEngine
 *
 *  Created by Quoc Le on 11/28/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */
#import "OGLES2Tools.h"
#import <string>

#ifndef _MESH_3D_H_
#define _MESH_3D_H_

class SceneGraph;
class Sprite3D;

typedef struct 
{
	int materialIndex;
	int vertexStride;
	int normalStride;
	int normalOffset;
	int tangentStride;
	int tangentOffset;	
	int uvStride;
	int uvOffset;
	
	int nNumFaces;
	int nNumStrips;
		
} MeshInfo;

typedef struct
{
	std::string textureFileName;
	PVRTVec3 diffuseColor;
	PVRTVec3 ambientColor;
	float	shiness;
	PVRTVec3 specularColor;
} MatInfo;

class Mesh3D 
{
	// 3D Model
	CPVRTModelPOD	m_ModelPOD;
	
	GLuint*         m_uiTexture;
	GLuint*         iVertexVBO;
	GLuint*         m_puiIndexVbo;
	GLuint*         iNormalVBO;
	GLuint*         m_normalVBO;
	GLuint*         iUVVBO;
	GLuint          m_normalMap;
	GLuint          m_cubeMap;
	
	int             m_numMeshes;
	int             m_numMaterials;	
	int             m_shader;

	MeshInfo*       meshInfo;
	bool            m_blendEnable;
	
	GLenum          m_blend1;
	GLenum          m_blend2;
	std::string     m_filename;
    
public:	
    
	Mesh3D(const char *filename);
	~Mesh3D();
    
	void drawMesh(int i32NodeIndex, int materialIndex);
	int getNumMeshes() { return m_numMeshes; } 
	
	void setTexture(int index, GLuint target);
	void setNormalMap(GLuint target) { m_normalMap = target; }
	void setCubeMap(GLuint target) { m_cubeMap = target; }
	
	void draw(SceneGraph *scene, Sprite3D *sprite);
	void draw(SceneGraph *scene, Sprite3D *sprite, int min, int max);

	void setShader(int shader) { m_shader = shader; }
	
	void setBlending(bool enable, GLenum blend1, GLenum blend2)
	{
		m_blendEnable = enable;
		m_blend1 = blend1;
		m_blend2 = blend2;
	}
    
    MatInfo         *materialInfo;
	bool            useSceneModel;
};

#endif