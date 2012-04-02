/*
 *  TextureManager.cpp
 *  Chess3D
 *
 *  Created by Quoc Le on 12/1/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */

#include "TextureManager.h"
#include "SceneGraph.h"



TextureManager TextureManager::_instance;

TextureManager &TextureManager::getInstance()
{
	return _instance;
}

GLint TextureManager::getTexture(const char* texture)
{
	map<std::string, GLint>::iterator it;

	it= m_textureMap.find(string(texture));
	
	if(it == m_textureMap.end())
	{
		GLuint ref = 0;
		
		if(PVRTTextureLoadFromPVR(texture, &ref) != PVR_SUCCESS){
			NSLog(@"Texture load failed for file %s", texture);
		}
		
		GLfloat maxAniso;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
		
		//NSLog(@"Anistropic %f", maxAniso);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		
		m_textureMap[ string(texture) ] = ref;
		//printf("  max=%d from file %d \n", maxAniso, ref);
		NSLog(@"Texture::getTexture |%s| ID=%d  ", texture,ref);
		
		return ref;
		
	} else {
		NSLog(@"Texture::getTexture |%s| ID=%d (CACHED) ", texture,it->second);

		return it->second;
	}

}

void TextureManager::removeTexture( const char* texture )
{
	map<std::string, GLint>::iterator it;
	
	it= m_textureMap.find(string(texture));
	
	if(it != m_textureMap.end())
	{
		GLuint tex = it->second;
		glDeleteTextures(1, &tex);				
		m_textureMap.erase(it);
	}
}