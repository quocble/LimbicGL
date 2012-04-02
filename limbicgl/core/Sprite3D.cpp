/*
 *  Sprite3D.cpp
 *  Chess3D
 *
 *  Created by Quoc Le on 12/1/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */

#include "Sprite3D.h"
#import "Mesh3D.h"
#import "SceneGraph.h"
#import "Tweeners.h"

void Sprite3D::moveToTarget(PVRTVec3 position, bool animate, float duration)
{
	// if animating, just jump to target position
	if( m_animating )
	{
		modelView = PVRTMat4::Translation( modelView.ptr()[12], modelView.ptr()[13], modelView.ptr()[14] ) * PVRTMat4::RotationY(rotations.y);
		m_animating = false;
	}
	//NSLog(@"moveToTarget %f %f %f", position.x, position.y, position.z);
	
	m_startTime = m_parent->PVRShellGetTime();
	m_duration = duration;
	m_startPosition = PVRTVec3(modelView.ptr()[12], modelView.ptr()[13], modelView.ptr()[14]);
	m_targetPosition = position;	
	m_animating = animate;
	
	if(!animate)
		modelView = PVRTMat4::Translation( position.x, position.y, position.z) * PVRTMat4::RotationY(rotations.y);
}	

void Sprite3D::draw()
{
	if(!m_model) return;
	
	if(m_textureId > -1)
		m_model->setTexture(0, m_textureId);
	
	m_model->setNormalMap(m_normalMap);
	m_model->setCubeMap(m_cubeMap);
	
	// translate
	
	if(m_animating)
	{
		unsigned long currentDuration = m_parent->PVRShellGetTime() - m_startTime;
		if(currentDuration > m_duration )
		{
			//NSLog(@"Animate stop %d - %d" , m_parent->PVRShellGetTime(), m_startTime);
			m_animating = false;
			modelView = PVRTMat4::Translation( m_targetPosition.x, m_targetPosition.y, m_targetPosition.z ) * PVRTMat4::RotationY(rotations.y);
		}
		else 
		{		
			float animateStep = Tweeners::tween(TWEEN_FUNC_EASEINSINE, currentDuration/float(m_duration));
			
			//NSLog(@"Animate step %f T(%d - %d) dur=%d", animateStep,  m_parent->PVRShellGetTime(), m_startTime, m_duration);
			
			float x = m_startPosition.x + (m_targetPosition.x - m_startPosition.x) * animateStep;
			float y = m_startPosition.y + (m_targetPosition.y - m_startPosition.y) * animateStep;
			float z = m_startPosition.z + (m_targetPosition.z - m_startPosition.z) * animateStep;
			modelView = PVRTMat4::Translation( x, y, z) * PVRTMat4::RotationY(rotations.y);
		}
	} 
	
	// draw
	if(m_enableDiffuse)
		m_model->materialInfo[0].diffuseColor = m_diffuseColor;
	if(m_enableAmbient)
		m_model->materialInfo[0].ambientColor = m_ambientColor;
	
	m_model->draw(m_parent, this);
}

void Sprite3D::setDiffuse(PVRTVec3 color)
{
	m_diffuseColor = color;
}

void Sprite3D::setAmbient(PVRTVec3 color)
{
	m_ambientColor = color;
}
