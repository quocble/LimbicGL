/*
 *  Drawable3D.h
 *  Chess3D
 *
 *  Created by Quoc Le on 12/1/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */
#import "PVRTModelPOD.h"
#import <list>
#import <algorithm>

typedef enum
{
	SHADER_ENVMAP,
	SHADER_VERTEX_LIGHTING,
	SHADER_DIFFUSE_BUMP_LIT,
	SHADER_UNLIT_TEXTURE,
	SHADER_ANISOTROPIC,
	SHADER_FASTTNL,
	SHADER_LATTICE,
	SHADER_POINT_LIGHTING,
	SHADER_PHONG_LIGHTING,
	SHADER_REFLECTIONS,
	SHADER_SIMPLE,
	SHADER_SPOTLIGHT,
	SHADER_TOON,
	SHADER_WOOD
} SHADER_T;


using namespace std;

class SceneGraph;

class Drawable3D
{
	
public:
    SceneGraph *m_parent;
    bool	 touchEnabled;

	PVRTMat4 modelView;
	PVRTVec3 rotations;	
	bool	 hidden;
	
	//pthread_mutex_t myLock;
	
	Drawable3D(SceneGraph *parent) : m_parent(parent), touchEnabled(false)
	{
		rotations = PVRTVec3(0,0,0);
		//pthread_mutex_init(&myLock, NULL);
		modelView = PVRTMat4::Identity();
		hidden = false;
	}
	~Drawable3D()
	{
		//pthread_mutex_destroy(&myLock);
	}
	
	void drawInternal();
	virtual void draw() = 0;
	void addChild(Drawable3D *drawable)  
	{ 
		if(drawable == 0)
		{
			printf("ERROR: ATTEMPT TO ADD NULL DRAWABLE");
			return;
		}
		//pthread_mutex_lock(&myLock);
		m_children.push_back(drawable); 
		//pthread_mutex_unlock(&myLock);
	} 
	void removeChild(Drawable3D *drawable) 
	{
		//pthread_mutex_lock(&myLock);
		
		list<Drawable3D*>::iterator it = find(m_children.begin(), m_children.end(), drawable) ;
		if(it != m_children.end())
		{
			m_children.erase(it);
		}
		
		//pthread_mutex_unlock(&myLock);
	}
	
	void removeAllChildren()
	{
		m_children.clear();
	}

	void touchBeganInternal(PVRTVec3 near, PVRTVec3 far);
	void touchMovedInternal(PVRTVec3 near, PVRTVec3 far);
	void touchEndedInternal(PVRTVec3 near, PVRTVec3 far);

	virtual void touchBegan(PVRTVec3 near, PVRTVec3 far) {}
	virtual void touchMoved(PVRTVec3 near, PVRTVec3 far) {}
	virtual void touchEnded(PVRTVec3 near, PVRTVec3 far) {}

protected:
	list<Drawable3D*> m_children;
};