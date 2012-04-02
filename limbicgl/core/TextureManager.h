/*
 *  TextureManager.h
 *  Chess3D
 *
 *  Created by Quoc Le on 12/1/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 *
 *  Manages all of the textures in the program. 
 *  Ensures that there is only 1 texture loaded & shared;
 */
#import "PVRTModelPOD.h"
#import <map>
#import <string>
using namespace std;

class TextureManager
{
private:
	static TextureManager _instance;
	
	TextureManager() {}
	~TextureManager() {} 
	TextureManager(const TextureManager &);             // intentionally undefined
	TextureManager & operator=(const TextureManager &); // intentionally undefined
	std::map<std::string, GLint>  m_textureMap; 
	
public:
	static TextureManager &getInstance();
	GLint getTexture(const char *pvrLocation);
	void removeTexture( const char* texture );
};