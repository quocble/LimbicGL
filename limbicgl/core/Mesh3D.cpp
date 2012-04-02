/*
 *  Mesh3D.cpp
 *  Chess3DEngine
 *
 *  Created by Quoc Le on 11/28/10.
 *  Copyright 2010 VisionaryBits, LLC. All rights reserved.
 *
 */

#include "Mesh3D.h"
#import "TextureManager.h"
#import "SceneGraph.h"
#import "OGLES2Tools.h"
#import "Sprite3D.h"

#define DEFAULT_SHADER SHADER_UNLIT_TEXTURE

/*!****************************************************************************
 @Function		LoadVbos
 @Description	Loads the mesh data required for this training course into
 vertex buffer objects
 ******************************************************************************/
Mesh3D::Mesh3D(const char *filename) :   m_normalMap(0), m_cubeMap(0), m_shader(DEFAULT_SHADER), m_blendEnable(false), useSceneModel(true)
{
	const char* cpath = [ [NSString stringWithFormat:@"%@/", [[NSBundle mainBundle] resourcePath]] cStringUsingEncoding: NSASCIIStringEncoding];
	
	NSLog(@"Loading model at %s ",filename);
	m_filename = filename;
	
	//printf("cpath %s/%@\n", cpath);
	
	CPVRTResourceFile::SetReadPath(cpath);
	if(m_ModelPOD.ReadFromFile(filename) != PVR_SUCCESS)
	{
		NSLog(@"ERROR: Couldn't load %s.", filename);
		return;
	} else {
		
		if(!m_ModelPOD.pMesh[0].pInterleaved)
		{
			printf("ERROR: IntroducingPOD requires the pod data to be interleaved. Please re-export with the interleaved option enabled.");
			return;
		}
		
		m_numMeshes = m_ModelPOD.nNumMesh;
		iVertexVBO = new GLuint[m_ModelPOD.nNumMesh];
		m_puiIndexVbo = new GLuint[m_ModelPOD.nNumMesh];
		meshInfo = new MeshInfo[m_ModelPOD.nNumMesh];

		/*
		 Load vertex data of all meshes in the scene into VBOs
		 
		 The meshes have been exported with the "Interleave Vectors" option,
		 so all data is interleaved in the buffer at pMesh->pInterleaved.
		 Interleaving data improves the memory access pattern and cache efficiency,
		 thus it can be read faster by the hardware.
		 */
		glGenBuffers(m_ModelPOD.nNumMesh, iVertexVBO);
		
		static int vertexCounts = 0;

		for (unsigned int i = 0; i < m_ModelPOD.nNumMesh; ++i)
		{
			// Load vertex data into buffer object
			SPODMesh& Mesh = m_ModelPOD.pMesh[i];
			//PVRTModelPODToggleInterleaved(Mesh);

			vertexCounts += Mesh.nNumVertex;
			
			unsigned int uiSize = Mesh.nNumVertex *  Mesh.sVertex.nStride;
			glBindBuffer(GL_ARRAY_BUFFER, iVertexVBO[i]);
			glBufferData(GL_ARRAY_BUFFER, uiSize, Mesh.pInterleaved, GL_STATIC_DRAW);

			//printf("Addr %x %x", Mesh.psUVW[0].pData, Mesh.sNormals.pData);
			
			meshInfo[i].vertexStride = Mesh.sVertex.nStride;
			meshInfo[i].materialIndex = m_ModelPOD.pNode[i].nIdxMaterial;
			meshInfo[i].normalStride =  Mesh.sNormals.nStride;
			meshInfo[i].normalOffset = (size_t) Mesh.sNormals.pData;
			meshInfo[i].tangentStride =  Mesh.sTangents.nStride;
			meshInfo[i].tangentOffset = (size_t) Mesh.sTangents.pData;
			
			meshInfo[i].uvStride =  Mesh.psUVW[0].nStride;
			meshInfo[i].uvOffset = (size_t) Mesh.psUVW[0].pData;
			
			meshInfo[i].nNumFaces =  Mesh.nNumFaces;
			meshInfo[i].nNumStrips = Mesh.nNumStrips;
			
			NSLog(@"Loading object name= %d vertices=%d TotalCount=%d", i,Mesh.nNumVertex, vertexCounts);

			// Load index data into buffer object if available
			m_puiIndexVbo[i] = 0;
			if (Mesh.sFaces.pData)
			{
				glGenBuffers(1, &m_puiIndexVbo[i]);
				uiSize = PVRTModelPODCountIndices(Mesh) * sizeof(GLshort);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_puiIndexVbo[i]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiSize, Mesh.sFaces.pData, GL_STATIC_DRAW);
				NSLog(@"Loading object name= %d indexes=%d", i,uiSize);
			}			
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		//load any textures for the scene's materials.
		m_uiTexture = new GLuint[m_ModelPOD.nNumMaterial];
		materialInfo = new MatInfo[m_ModelPOD.nNumMaterial];
		m_numMaterials = m_ModelPOD.nNumMaterial;
		
		for(unsigned int i=0;i<m_ModelPOD.nNumMaterial;i++){
			m_uiTexture[i] = INT_MAX;

			SPODMaterial* mat = &m_ModelPOD.pMaterial[i];

			materialInfo[i].diffuseColor = mat->pfMatDiffuse;
			materialInfo[i].ambientColor = mat->pfMatAmbient;
			materialInfo[i].shiness = mat->fMatShininess;
			materialInfo[i].specularColor = mat->pfMatSpecular;
			
			if(mat->nIdxTexDiffuse != -1){
				//load the texture file.
				CPVRTString filenamePVR = m_ModelPOD.pTexture[mat->nIdxTexDiffuse].pszName;
				m_uiTexture[i] = TextureManager::getInstance().getTexture( filenamePVR.c_str() );

				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				//glMaterialxv(GL_FRONT_AND_BACK, GL_AMBIENT, PVRTVec4(mat->pfMatAmbient, f2vt(1.0f)).ptr()); 
				//glMaterialxv(GL_FRONT_AND_BACK, GL_DIFFUSE, PVRTVec4(mat->pfMatDiffuse, f2vt(1.0f)).ptr()); 
				
				materialInfo[i].textureFileName = filenamePVR.c_str();
				
				NSLog(@"Loaded texture %s %d", filenamePVR.c_str(), m_uiTexture[i]);
			}
			else 
			{
				materialInfo[i].textureFileName = "";
				//NSLog(@"Skipping texture");
			}
		}
	}
	m_ModelPOD.Destroy();
}

Mesh3D::~Mesh3D()
{
	NSLog(@"Destroy %s", m_filename.c_str());
	glDeleteBuffers(m_ModelPOD.nNumMesh, iVertexVBO);
	glDeleteBuffers(m_ModelPOD.nNumMesh, m_puiIndexVbo);
	
	delete[] iVertexVBO;
	delete[] m_puiIndexVbo;
	delete[] meshInfo;	
	
	for(int i=0;i< m_numMaterials ;i++)
	{
		TextureManager::getInstance().removeTexture( materialInfo[i].textureFileName.c_str() );
	}
	delete[] materialInfo;
	delete[] m_uiTexture;
}

void Mesh3D::setTexture(int index, GLuint target)
{
	m_uiTexture[index] = target;
}

void Mesh3D::draw(SceneGraph *scene, Sprite3D *sprite)
{
	draw(scene, sprite, 0, getNumMeshes());
}
void Mesh3D::draw(SceneGraph *scene,  Sprite3D *sprite, int min, int max)
{
	
	scene->m_ppEffect[m_shader]->Activate();
	
	if(m_blendEnable)
	{
		glEnable (GL_BLEND);
		glBlendFunc (m_blend1, m_blend2);
	} else 
	{
		glDisable (GL_BLEND);
	}
	
	//glEnable(GL_SAMPLE_COVERAGE);
	//glSampleCoverage(1.0, GL_FALSE);
	
	for(unsigned int j = 0; j < scene->m_pnUniformCnt[m_shader]; ++j)
	{
		//unsigned int location = scene->m_ppsUniforms[m_shader][j].nLocation;
		EUniformSemantic semantic = (EUniformSemantic)scene->m_ppsUniforms[m_shader][j].nSemantic; 
		switch(semantic)
		{
			case eUsMVPMATRIX:
			{
				PVRTMat4 mMVP;
				
				/* Passes the model-view-projection matrix (MVP) to the shader to transform the vertices */
				
				if(useSceneModel)
				{
					mMVP = scene->m_mProjection * scene->m_mModelView * sprite->modelView;
				}
				else 
				{
					mMVP = scene->m_mProjection * sprite->modelView;
				}
				glUniformMatrix4fv(scene->m_ppsUniforms[m_shader][j].nLocation, 1, GL_FALSE, mMVP.f);
			}
				break;
			case eUsMODELVIEW:
			{
				PVRTMat4 MV = useSceneModel ? scene->m_mModelView * sprite->modelView : sprite->modelView  ;
				glUniformMatrix4fv(scene->m_ppsUniforms[m_shader][j].nLocation, 1, GL_FALSE, MV.f);
			}
				break;
			case eUsMODELVIEWIT:
			{
				PVRTMat4 mModelViewI, mModelViewIT;
				PVRTMat4 MV = useSceneModel ?  scene->m_mModelView * sprite->modelView : sprite->modelView ;
				/* Passes the inverse transpose of the model-view matrix to the shader to transform the normals */
				mModelViewI = MV.inverse();
				mModelViewIT= mModelViewI.transpose();
				PVRTMat3 ModelViewIT = PVRTMat3(mModelViewIT);
				
				glUniformMatrix3fv(scene->m_ppsUniforms[m_shader][j].nLocation, 1, GL_FALSE, ModelViewIT.f);
			}
				break;
			case eUsVIEWIT:
			{
				PVRTMat4 mViewI, mViewIT;
				
				/* Passes the inverse transpose of the model-view matrix to the shader to transform the normals */
				mViewI = scene->m_mView.inverse();
				mViewIT= mViewI.transpose();
				
				PVRTMat3 ViewIT = PVRTMat3(mViewIT);
				
				glUniformMatrix3fv(scene->m_ppsUniforms[m_shader][j].nLocation, 1, GL_FALSE, ViewIT.f);
			}
				break;	
                
            default:
                break;

		}
	}
	
	
	for(int i32MeshIndex  =min; i32MeshIndex < max; i32MeshIndex++)
	{
		//int i32MeshIndex = i;
		//int i32MeshIndex = m_ModelPOD.pNode[i].nIdx;
		//SPODMesh* submesh = &m_ModelPOD.pMesh[i32MeshIndex];
		//int materialIndex = m_ModelPOD.pNode[i].nIdxMaterial;
		//SPODMaterial* pMaterial = &m_ModelPOD.pMaterial[materialIndex];
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_puiIndexVbo[i32MeshIndex]);
		int materialIndex = meshInfo[i32MeshIndex].materialIndex;
		
		for(unsigned int j = 0; j < scene->m_pnUniformCnt[m_shader]; ++j)
		{
			unsigned int location = scene->m_ppsUniforms[m_shader][j].nLocation;
			EUniformSemantic semantic = (EUniformSemantic)scene->m_ppsUniforms[m_shader][j].nSemantic; 
			switch(semantic)
			{
				case eUsMATERIALCOLORAMBIENT:
				{
					PVRTVec3 vColour = materialInfo[materialIndex].ambientColor;
					glUniform3f(location, vColour.x, vColour.y, vColour.z);
				}
					break;
				case eUsMATERIALCOLORDIFFUSE:
				{
					PVRTVec3 vColour =  materialInfo[materialIndex].diffuseColor;
					glUniform3f(location, vColour.x, vColour.y, vColour.z);
				}
					break;
					
				case eUsPOSITION:
				{
					glBindBuffer(GL_ARRAY_BUFFER, iVertexVBO[ i32MeshIndex] );
					//glVertexAttribPointer(m_ppsUniforms[m_nCurrentShader][j].nLocation, 3, GL_FLOAT, GL_FALSE, 0, (const void*) NULL);
					glVertexAttribPointer(scene->m_ppsUniforms[m_shader][j].nLocation, 3, GL_FLOAT, GL_FALSE, meshInfo[i32MeshIndex].vertexStride, (const void*) NULL);
					
					glEnableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
				}
					break;
				case eUsNORMAL:
				{
					glBindBuffer(GL_ARRAY_BUFFER, iVertexVBO[ i32MeshIndex]);
					glVertexAttribPointer(scene->m_ppsUniforms[m_shader][j].nLocation, 3, GL_FLOAT, GL_FALSE, 
										  meshInfo[i32MeshIndex].normalStride, (const void*)  meshInfo[i32MeshIndex].normalOffset);
					
					glEnableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
				}
					break;
				case eUsTANGENT:
				{
					glBindBuffer(GL_ARRAY_BUFFER, iVertexVBO[ i32MeshIndex]);
					glVertexAttribPointer(scene->m_ppsUniforms[m_shader][j].nLocation, 3, GL_FLOAT, GL_FALSE, 
										  meshInfo[i32MeshIndex].tangentStride, (const void*)  meshInfo[i32MeshIndex].tangentOffset);
					
					glEnableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
					
				}
					break;
				case eUsUV:
				{
					
					//glVertexAttribPointer(m_ppsUniforms[m_nCurrentShader][j].nLocation, 2, GL_FLOAT, GL_FALSE, 0, (const void*) NULL);
					if( meshInfo[i32MeshIndex].uvOffset != 0)
					{
						glBindBuffer(GL_ARRAY_BUFFER, iVertexVBO[i32MeshIndex]);
						glVertexAttribPointer(scene->m_ppsUniforms[m_shader][j].nLocation, 2, GL_FLOAT, GL_FALSE, 
											  meshInfo[i32MeshIndex].uvStride, (const void*)  meshInfo[i32MeshIndex].uvOffset);
						glEnableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
					}
				}
					break;

				case eUsTEXTURE:
				{
					// Set the sampler variable to the texture unit					
					int index = scene->m_ppsUniforms[m_shader][j].nIdx;
					
					switch(index)
					{
						case 0:
						{
							GLuint tex = m_uiTexture[materialIndex];
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, tex);
							
							//NSLog(@"Normal map %d %d", index, tex);
							glUniform1i(scene->m_ppsUniforms[m_shader][j].nLocation, index);
						}
							break;
						case 1:
						{
							//NSLog(@"Normal map %d %d", index, m_normalMap);
							glActiveTexture(GL_TEXTURE1);
							glBindTexture(GL_TEXTURE_2D, m_normalMap);
						
							glUniform1i(scene->m_ppsUniforms[m_shader][j].nLocation, index);
						}
							break;
						case 2:
						{
							glActiveTexture(GL_TEXTURE2);
							glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

							glUniform1i(scene->m_ppsUniforms[m_shader][j].nLocation, index);
						}
							break;
					}
					
				}				
					break;
					
				case eUsTEXTURE_ENABLED:
				{
					int texture_enabled = 0;
					int texture = (signed int)m_uiTexture[materialIndex];
					if(texture != INT_MAX)
						texture_enabled = 1;
						
					//printf("Texture %d %d\n", m_uiTexture[materialIndex], texture_enabled);
					if(m_normalMap > 0)
					{
						texture_enabled |= 2;
					}
					if(m_cubeMap > 0)
					{
						texture_enabled |= 4;
					}
					
					glUniform1i(scene->m_ppsUniforms[m_shader][j].nLocation, texture_enabled);
				}
					break;
				case eUsANIMATION:
				{
					// Float in the range 0..1: contains this objects distance through its animation.
					float fAnimation = 0.5f * scene->m_fViewAngle / PVRT_PI;
					glUniform1f(scene->m_ppsUniforms[m_shader][j].nLocation, fAnimation);
				}
					break;
				case eUsMATERIALSHININESS:
				{
					float shiness =  materialInfo[materialIndex].shiness;
					glUniform1f(location, shiness);
				}
					break;
				case eUsMATERIALCOLORSPECULAR:
				{
					PVRTVec3 vColour =  materialInfo[materialIndex].specularColor;
					glUniform3f(location, vColour.x, vColour.y, vColour.z);
				}
					break;
				case eUsLIGHTPOSWORLD:
				{
					PVRTVec3 position(45, 72, 52);
					glUniform3f(location, position.x , position.y, position.z);
				}
					break;
                default:
                    break;
			}
		}
		// Load the correct texture using our texture lookup table
		
		
		//glBindBuffer(GL_ARRAY_BUFFER, 0);	// Unbind the last buffer used.
		drawMesh(i32MeshIndex, materialIndex);
	}
	
	/*
	 Disable attributes
	 */
	for(unsigned int j = 0; j < scene->m_pnUniformCnt[m_shader]; ++j)
	{
		switch(scene->m_ppsUniforms[m_shader][j].nSemantic)
		{
			case eUsPOSITION:
			{
				glDisableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
			}
				break;
			case eUsNORMAL:
			{
				glDisableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
			}
				break;
			case eUsUV:
			{
				glDisableVertexAttribArray(scene->m_ppsUniforms[m_shader][j].nLocation);
			}
				break;
		
		}
	}

	glDisable(GL_BLEND);

}
//#include "PVRTFixedPointAPI.h"

/*!****************************************************************************
 @Function		DrawMesh
 @Input			i32NodeIndex		Node index of the mesh to draw
 @Description	Draws a SPODMesh after the model view matrix has been set and
 the meterial prepared.
 ******************************************************************************/
void Mesh3D::drawMesh(int i32MeshIndex, int materialIndex)
{
	
/*
	 The geometry can be exported in 4 ways:
	 - Indexed Triangle list
	 - Non-Indexed Triangle list
	 - Indexed Triangle strips
	 - Non-Indexed Triangle strips
	 */
	if( meshInfo[i32MeshIndex].nNumStrips == 0)
	{
		if(m_puiIndexVbo[i32MeshIndex])
		{
			// Indexed Triangle list
			glDrawElements(GL_TRIANGLES,  meshInfo[i32MeshIndex].nNumFaces*3, GL_UNSIGNED_SHORT, 0);
		}
		else
		{
			// Non-Indexed Triangle list
			glDrawArrays(GL_TRIANGLES, 0,  meshInfo[i32MeshIndex].nNumFaces*3);
		}
	}
	else
	{
		for(int i = 0; i < (int) meshInfo[i32MeshIndex].nNumStrips; ++i)
		{
			//int offset = 0;
			if(m_puiIndexVbo[i32MeshIndex])
			{
				// Indexed Triangle strips
				//glDrawElements(GL_TRIANGLE_STRIP, pMesh->pnStripLength[i]+2, GL_UNSIGNED_SHORT, (GLshort*)(offset*2));
			}
			else
			{
				// Non-Indexed Triangle strips
				//glDrawArrays(GL_TRIANGLE_STRIP, offset, pMesh->pnStripLength[i]+2);
			}
			//offset += pMesh->pnStripLength[i]+2;
		}
	}
	

	// Safely disable the vertex attribute arrays
	//glDisableVertexAttribArray(VERTEX_ARRAY);
	//glDisableVertexAttribArray(NORMAL_ARRAY);
	//glDisableVertexAttribArray(TEXCOORD_ARRAY);
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

