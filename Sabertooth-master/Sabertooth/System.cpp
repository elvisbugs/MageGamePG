#include "System.h"

System::System()
{
}


System::~System()
{
}

void System::delay(int milliSeconds)
{
	clock_t start_time = clock();
	while (clock() < start_time + milliSeconds);
};

int System::GLFWInit()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	window = glfwCreateWindow( WIDTH, HEIGHT, "Sabertooth", nullptr, nullptr );

	glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

	if ( window == nullptr ) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;

	if ( glewInit() != GLEW_OK ) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport( 0, 0, screenWidth, screenHeight );

	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{

	glEnable( GL_BLEND );	// Enables blending ( glBlendFunc )
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_DEPTH_TEST );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	coreShader = Shader( "Shaders/Core/core.vert", "Shaders/Core/core.frag", 3.42, 2, -0.1f, -1.0, -0.7);
	coreShader2 = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", 1.76, 0.64, -1.0f);
	ceu = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", 2, 2, 0.0f, -1.0, -0.8);
	nuvem = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", 2, 0.33, -0.01f,-1.0,0.65);
	grama = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", 3.42, 2, -0.9f);

	coreShader.useShader();
	coreShader2.useShader();
	ceu.useShader();
	nuvem.useShader();
	grama.useShader();

	return EXIT_SUCCESS;
}

void System::Run()
{
	//load grama
	grama.useShader();
	grama.loadTexture("bin/assets/Background/grama.png", "texture1", "layerGrama", true);
	if (!grama.bindVAO())
		return;

	//load nuvem
	nuvem.useShader();
	nuvem.loadTexture("bin/assets/Background/nuvens.png", "texture1", "layerNuvem", true);
	if (!nuvem.bindVAO())
		return;

	//load ceu
	ceu.useShader();
	ceu.loadTexture("bin/assets/Background/layer1.png", "texture1", "layer0", true);
	if (!ceu.bindVAO())
		return;

	//load mountains
	coreShader.useShader();
	coreShader.loadTexture( "bin/assets/Background/layer0.png", "texture1", "layer1", true );
	if (!coreShader.bindVAO())
		return;
	
	//load mages
	coreShader2.useShader();
	coreShader2.loadTexture("bin/assets/Mage/mage_fire.png", "texture1", "mage_fire", false);
	coreShader2.loadTexture("bin/assets/Mage/mage_water.png", "texture2", "mage_water", false);
	coreShader2.loadTexture("bin/assets/Mage/mage_wind.png", "texture3", "mage_wind", false);
	if (!coreShader2.bindVAO())
		return;
	
	int teste = 1;
	float offsetx = 0;

	while ( !glfwWindowShouldClose( window ) ) {

		glfwPollEvents();

#pragma region Input Handling

		if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
			glfwSetWindowShouldClose( window, GLFW_TRUE );
		}

#pragma endregion

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


		//ceu update
		ceu.useShader();
		ceu.useTexture("layer0");
		glBindVertexArray(ceu.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//grama update
		grama.useShader();
		glUniform1f(glGetUniformLocation(grama.getProgramId(), "offsetx"), offsetx);
		grama.useTexture("layerGrama");
		glBindVertexArray(grama.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//nuvem update
		nuvem.useShader();
		glUniform1f(glGetUniformLocation(nuvem.getProgramId(), "offsetx"), offsetx/100);
		nuvem.useTexture("layerNuvem");
		glBindVertexArray(nuvem.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//background update
		coreShader.useShader();
		glUniform1f(glGetUniformLocation(coreShader.getProgramId(), "offsetx"), offsetx/60);
		coreShader.useTexture( "layer1" );
		glBindVertexArray(coreShader.getVAO());
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray( 0 );

		//Mage update
		coreShader2.useShader();
		
		if (teste == 1) {
			GLuint textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 1);
			coreShader2.useTexture("mage_fire");
			glBindVertexArray(coreShader2.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		else if (teste == 2)
		{
			GLuint textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			coreShader2.useTexture("mage_water");
			glBindVertexArray(coreShader2.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		else
		{
			GLuint textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			coreShader2.useTexture("mage_wind");
			glBindVertexArray(coreShader2.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		if (teste == 1) teste = 2;
		else if (teste == 2) teste = 3;
		else teste = 1; 

		if (offsetx >= 2147483647)
		{
			offsetx = 0.0;
		}
		else if (offsetx <= -2147483647)
		{
			offsetx = 0.0;
		}
		else
		{
			offsetx += 0.1;
		}

		//delay(300);
		
		glfwSwapBuffers( window );
	}
}

void System::Finish()
{
	coreShader.deleteShader();
	glfwTerminate();
}
