#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include "Texture.h"
#include <glm/glm.hpp>


class Shader
{

private: 	
	GLuint mIntProgramId;

	GLuint mIntEBO;
	GLuint mIntVAO;
	GLuint mIntVBO;

	GLuint mIntTexture;

	std::map<std::string, Texture> textures;
	GLint textureQtd;

	GLfloat mFltVertices[30];

public:

	Shader() { textureQtd = 0; }
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* pChrVertexPath, const GLchar* pChrFragmentPath, GLfloat pFltWidth, GLfloat pFltHeigth, GLfloat pFltLayer);
	Shader(const GLchar* pChrVertexPath, const GLchar* pChrFragmentPath, GLfloat pFltWidth, GLfloat pFltHeigth, GLfloat pFltLayer, GLfloat pFltPosX, GLfloat pFltPosY);

	~Shader();

	void useShader(){glUseProgram( this->mIntProgramId);}

	void deleteShader() { glDeleteProgram(this->mIntProgramId);}

	void useTexture( std::string textureName );
	void loadTexture( char* path, char* textureUniformName, std::string textureName, bool pBlnRepeat);

	bool bindVAO();
	GLuint getVAO() { return mIntVAO;}

	GLuint getProgramId() { return mIntProgramId; }
};

#endif