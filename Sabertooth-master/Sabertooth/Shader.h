#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include "Texture.h"


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

public:
	GLfloat mFltVertices[30];

	Shader() { textureQtd = 0; }
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* pChrVertexPath, const GLchar* pChrFragmentPath, GLfloat pIntHeigth, GLfloat pIntWidth, GLfloat pFltLayer);

	~Shader();

	void Use(){glUseProgram( this->mIntProgramId);}

	void Delete() { glDeleteProgram(this->mIntProgramId);}

	void UseTexture( std::string textureName );
	void LoadTexture( char* path, char* textureUniformName, std::string textureName );

	bool BindVAO();
	GLuint getVAO() { return this->mIntVAO;}
};

#endif