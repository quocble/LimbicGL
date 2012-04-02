/*
 *  World.cpp
 *  Chess3D
 *
 *  Created by Quoc Le on 12/1/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */

#include "Drawable3D.h"

void Drawable3D::drawInternal()
{
	if(hidden)  return;
	
	//pthread_mutex_lock(&myLock);

	draw();
	
	list<Drawable3D*>::iterator it;
	
	for ( it= m_children.begin() ; it != m_children.end(); it++ )
	{
		//printf("Drawing child %d", *it - *m_children.begin());
		(*it)->drawInternal();
	}
	
	//pthread_mutex_unlock(&myLock);
}

void Drawable3D::touchBeganInternal(PVRTVec3 near, PVRTVec3 far)
{
	//pthread_mutex_lock(&myLock);

	if( touchEnabled )
	{
		touchBegan(near, far);
	}	

	list<Drawable3D*>::iterator it;
	for ( it= m_children.begin() ; it != m_children.end(); it++ )
	{
		if( (*it)->touchEnabled )
		{
			(*it)->touchBeganInternal(near,far);
		}
	}
	
	//pthread_mutex_unlock(&myLock);
}

void Drawable3D::touchMovedInternal(PVRTVec3 near, PVRTVec3 far)
{
	touchMoved(near,far);
	
	list<Drawable3D*>::iterator it;
	
	for ( it= m_children.begin() ; it != m_children.end(); it++ )
	{
		(*it)->touchMovedInternal(near,far);
	}
}

void Drawable3D::touchEndedInternal(PVRTVec3 near, PVRTVec3 far)
{
	touchEnded(near,far);
	
	list<Drawable3D*>::iterator it;
	
	for ( it= m_children.begin() ; it != m_children.end(); it++ )
	{
		(*it)->touchEndedInternal(near,far);
	}
}

