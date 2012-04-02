/*
 *  Sprite3D.h
 *  Chess3D
 *
 *  Created by Quoc Le on 12/1/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */

#import "Drawable3D.h"
#include "Mesh3D.h"

class SceneGraph;

class Sprite3D : public Drawable3D
{
public:
	Sprite3D(): Drawable3D(0),m_textureId(-1),m_Transparent(0) {}
	Sprite3D(SceneGraph *parent): Drawable3D(parent) , m_textureId(-1), m_animating(false), 
                                            m_model(0),m_Transparent(0) , m_enableDiffuse(false)
	{}
	Sprite3D(SceneGraph *parent, Mesh3D *model, GLuint textureId): Drawable3D(parent) ,  
                                            m_textureId(textureId), m_animating(false), m_model(model),
	m_Transparent(0), m_enableDiffuse(false)
	{}
	
	void setModel( Mesh3D *mesh) {  m_model = mesh; }
	void setTexture( GLuint textureId)  { m_textureId = textureId; }
	void setDiffuse(PVRTVec3 color);
	void enableDiffuseColor(bool enable) { m_enableDiffuse = enable; }
	void setAmbient(PVRTVec3 color);
	void setSpecular(PVRTVec3 color) { m_specularColor = color; }
	void enableAmbientColor(bool enable) { m_enableAmbient = enable; }	
	
	void setNormalMap( GLuint target) { m_normalMap = target; }
	void setCubeMap( GLuint target) { m_cubeMap = target; }
					  
 	virtual void draw();
	
	virtual void touchBegan(PVRTVec3 near, PVRTVec3 far) {}
	virtual void touchMoved(PVRTVec3 near, PVRTVec3 far) {}
	virtual void touchEnded(PVRTVec3 near, PVRTVec3 far) {}
	void moveToTarget(PVRTVec3 position, bool animate, float duration);
	
	
protected:
	int m_textureId;
	bool m_animating;
    	
	Mesh3D *m_model;
	bool m_Transparent;
	bool m_enableDiffuse;
	bool m_enableAmbient;
	GLuint m_normalMap;
	GLuint m_cubeMap;
	
    unsigned long m_duration;
	unsigned long m_startTime;
	PVRTVec3 m_startPosition;
	PVRTVec3 m_targetPosition;
	PVRTVec3 m_diffuseColor;
	PVRTVec3 m_ambientColor;
	PVRTVec3 m_specularColor;

};