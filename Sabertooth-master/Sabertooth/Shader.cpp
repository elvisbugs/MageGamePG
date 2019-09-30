#include "Shader.h"


Shader::~Shader()
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) : Shader() {

	// Get vertex and fragment shaders source codes from files' paths
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	// Ensure ifstream objects can throw exceptions
	vertexShaderFile.exceptions(std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexPath);
		if (!vertexShaderFile.is_open()) {
			std::cout << "ERROR::SHADER::VERTEX_SHADER_PATH" << std::endl;
		}

		fragmentShaderFile.open(fragmentPath);
		if (!fragmentShaderFile.is_open()) {
			std::cout << "ERROR::SHADER::FRAGMENT_SHADER_PATH" << std::endl;
		}

		std::stringstream vShaderStream, fShaderStream;

		// Read files' contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (const std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE NOT SUCCESUFULLY READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// Compile Shaders

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Shader Program
	this->mIntProgramId = glCreateProgram();
	glAttachShader(this->mIntProgramId, vertex);
	glAttachShader(this->mIntProgramId, fragment);
	glLinkProgram(this->mIntProgramId);

	// Print Linking errors if there any
	glGetProgramiv(this->mIntProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->mIntProgramId, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete shaders (already linked, they're no longer necessary)
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const GLchar* pChrVertexPath, const GLchar* pChrFragmentPath, GLfloat pFltWidth, GLfloat pFltHeigth, GLfloat pFltLayer) : Shader(pChrVertexPath, pChrFragmentPath)
{
	//monta os vertices do shader
	this->mFltVertices[0] = -1 + pFltWidth;			// x->top right
	this->mFltVertices[1] = -1 + pFltHeigth;		// y->top right
	this->mFltVertices[2] =  pFltLayer;				// z->top right

	this->mFltVertices[5] = -1 + pFltWidth;			// x->bottom right
	this->mFltVertices[6] = -1;						// y->bottom right
	this->mFltVertices[7] = pFltLayer;				// z->bottom right

	this->mFltVertices[10] = -1;					// x->bottom left
	this->mFltVertices[11] = -1;					// y->bottom left
	this->mFltVertices[12] = pFltLayer;				// z->bottom left

	this->mFltVertices[15] = this->mFltVertices[10];// x->bottom left
	this->mFltVertices[16] = this->mFltVertices[11];// y->bottom left
	this->mFltVertices[17] = this->mFltVertices[12];// z->bottom left

	this->mFltVertices[20] = -1;					// x->top left
	this->mFltVertices[21] = -1 + pFltHeigth;		// y->top left
	this->mFltVertices[22] = pFltLayer;				// z->top left

	this->mFltVertices[25] = this->mFltVertices[0];	// x->top right
	this->mFltVertices[26] = this->mFltVertices[1];	// y->top right
	this->mFltVertices[27] = this->mFltVertices[2];	// z->top right

	//monta a posições das texturas
	this->mFltVertices[3] = 1.0f;				    //x->top right
	this->mFltVertices[4] = 1.0f;				    //y->top right
												    
	this->mFltVertices[8] = 1.0f;				    // x->bottom right
	this->mFltVertices[9] = 0.0f;				    // y->bottom right
													    
	this->mFltVertices[13] = 0.0f;				    // x->bottom left
	this->mFltVertices[14] = 0.0f;				    // y->bottom left
												   
	this->mFltVertices[18] = this->mFltVertices[13];// x->bottom left
	this->mFltVertices[19] = this->mFltVertices[14];// y->bottom left
												   
	this->mFltVertices[23] = 0.0f;				    // x->top left
	this->mFltVertices[24] = 1.0f;				    // y->top left

	this->mFltVertices[28] = this->mFltVertices[3];//x->top right
	this->mFltVertices[29] = this->mFltVertices[4];//y->top right
}

Shader::Shader(const GLchar* pChrVertexPath, const GLchar* pChrFragmentPath, GLfloat pFltWidth, GLfloat pFltHeigth, GLfloat pFltLayer, GLfloat pFltPosX, GLfloat pFltPosY) : Shader(pChrVertexPath, pChrFragmentPath)
{
	//monta os vertices do shader
	this->mFltVertices[0] = pFltPosX + pFltWidth;			// x->top right
	this->mFltVertices[1] = pFltPosY + pFltHeigth;		// y->top right
	this->mFltVertices[2] = pFltLayer;				// z->top right

	this->mFltVertices[5] = pFltPosX + pFltWidth;			// x->bottom right
	this->mFltVertices[6] = pFltPosY;						// y->bottom right
	this->mFltVertices[7] = pFltLayer;				// z->bottom right

	this->mFltVertices[10] = pFltPosX;					// x->bottom left
	this->mFltVertices[11] = pFltPosY;					// y->bottom left
	this->mFltVertices[12] = pFltLayer;				// z->bottom left

	this->mFltVertices[15] = this->mFltVertices[10];// x->bottom left
	this->mFltVertices[16] = this->mFltVertices[11];// y->bottom left
	this->mFltVertices[17] = this->mFltVertices[12];// z->bottom left

	this->mFltVertices[20] = pFltPosX;					// x->top left
	this->mFltVertices[21] = pFltPosY + pFltHeigth;		// y->top left
	this->mFltVertices[22] = pFltLayer;				// z->top left

	this->mFltVertices[25] = this->mFltVertices[0];	// x->top right
	this->mFltVertices[26] = this->mFltVertices[1];	// y->top right
	this->mFltVertices[27] = this->mFltVertices[2];	// z->top right

	//monta a posições das texturas
	this->mFltVertices[3] = 1.0f;				    //x->top right
	this->mFltVertices[4] = 1.0f;				    //y->top right

	this->mFltVertices[8] = 1.0f;				    // x->bottom right
	this->mFltVertices[9] = 0.0f;				    // y->bottom right

	this->mFltVertices[13] = 0.0f;				    // x->bottom left
	this->mFltVertices[14] = 0.0f;				    // y->bottom left

	this->mFltVertices[18] = this->mFltVertices[13];// x->bottom left
	this->mFltVertices[19] = this->mFltVertices[14];// y->bottom left

	this->mFltVertices[23] = 0.0f;				    // x->top left
	this->mFltVertices[24] = 1.0f;				    // y->top left

	this->mFltVertices[28] = this->mFltVertices[3];//x->top right
	this->mFltVertices[29] = this->mFltVertices[4];//y->top right
}

Shader::Shader(const GLchar* pChrVertexPath, const GLchar* pChrFragmentPath, GLfloat pFltWidth, GLfloat pFltHeigth, GLfloat pFltLayer, GLfloat pFltPosX, GLfloat pFltPosY, GLfloat pFltTexX) : Shader(pChrVertexPath, pChrFragmentPath)
{
	//monta os vertices do shader
	this->mFltVertices[0] = pFltPosX + pFltWidth;			// x->top right
	this->mFltVertices[1] = pFltPosY + pFltHeigth;		// y->top right
	this->mFltVertices[2] = pFltLayer;				// z->top right

	this->mFltVertices[5] = pFltPosX + pFltWidth;			// x->bottom right
	this->mFltVertices[6] = pFltPosY;						// y->bottom right
	this->mFltVertices[7] = pFltLayer;				// z->bottom right

	this->mFltVertices[10] = pFltPosX;					// x->bottom left
	this->mFltVertices[11] = pFltPosY;					// y->bottom left
	this->mFltVertices[12] = pFltLayer;				// z->bottom left

	this->mFltVertices[15] = this->mFltVertices[10];// x->bottom left
	this->mFltVertices[16] = this->mFltVertices[11];// y->bottom left
	this->mFltVertices[17] = this->mFltVertices[12];// z->bottom left

	this->mFltVertices[20] = pFltPosX;					// x->top left
	this->mFltVertices[21] = pFltPosY + pFltHeigth;		// y->top left
	this->mFltVertices[22] = pFltLayer;				// z->top left

	this->mFltVertices[25] = this->mFltVertices[0];	// x->top right
	this->mFltVertices[26] = this->mFltVertices[1];	// y->top right
	this->mFltVertices[27] = this->mFltVertices[2];	// z->top right

	//monta a posições das texturas
	this->mFltVertices[3] = pFltTexX;				    //x->top right
	this->mFltVertices[4] = 1;				    //y->top right

	this->mFltVertices[8] = pFltTexX;				    // x->bottom right
	this->mFltVertices[9] = 0.0f;				    // y->bottom right

	this->mFltVertices[13] = 0.0f;				    // x->bottom left
	this->mFltVertices[14] = 0.0f;				    // y->bottom left

	this->mFltVertices[18] = this->mFltVertices[13];// x->bottom left
	this->mFltVertices[19] = this->mFltVertices[14];// y->bottom left

	this->mFltVertices[23] = 0.0f;				    // x->top left
	this->mFltVertices[24] = 1;				    // y->top left

	this->mFltVertices[28] = this->mFltVertices[3];//x->top right
	this->mFltVertices[29] = this->mFltVertices[4];//y->top right
}

bool Shader::bindVAO()
{
	try
	{
		glGenVertexArrays(1, &this->mIntVAO);
		glGenBuffers(1, &this->mIntVBO);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(this->mIntVAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->mIntVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->mFltVertices), this->mFltVertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0); // Unbind VAO

		return true;
	}
	catch (...)
	{
		std::cout << "Failed to Bind VAO!" << std::endl;
		return false;
	}
	
}

void Shader::useTexture( std::string textureName )
{
	glActiveTexture( GL_TEXTURE0 + textures[textureName].GetTextureNum() );
	glBindTexture( GL_TEXTURE_2D, textures[textureName].GetTextureId() );
}

void Shader::loadTexture( char* path, char* textureUniformName, std::string textureName, bool pBlnRepeat)
{
	Texture tempTexture;
	tempTexture.Load( path, textureUniformName, mIntProgramId, textureQtd, pBlnRepeat);
	textures[textureName] = tempTexture;
	textureQtd += 1;
}
