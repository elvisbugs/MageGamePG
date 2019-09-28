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

	coreShader = Shader( "Shaders/Core/core.vert", "Shaders/Core/core.frag", 8.46, 2, 0.0f);

	coreShader2 = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", 1.76, 0.64, -1.0f);

	coreShader.useShader();

	coreShader2.useShader();

	return EXIT_SUCCESS;
}

void System::Run()
{
	coreShader.useShader();

	coreShader.loadTexture( "bin/assets/Background/layer_0.png", "texture1", "layer_0" );

	if (!coreShader.bindVAO())
		return;

	coreShader2.useShader();
	coreShader2.loadTexture("bin/assets/Mage/mage_fire.png", "texture1", "mage_fire");
	coreShader2.loadTexture("bin/assets/Mage/mage_water.png", "texture2", "mage_water");
	coreShader2.loadTexture("bin/assets/Mage/mage_wind.png", "texture3", "mage_wind");

	if (!coreShader2.bindVAO())
		return;
	
	int teste = 1;

	while ( !glfwWindowShouldClose( window ) ) {

		glfwPollEvents();

#pragma region Input Handling

		if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
			glfwSetWindowShouldClose( window, GLFW_TRUE );
		}

#pragma endregion

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//background update
		coreShader.useShader();
		coreShader.useTexture( "layer_0" );
		glBindVertexArray(coreShader.getVAO());
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray( 0 );

		//Mage update
		coreShader2.useShader();
		
		if (teste == 1) {
			GLuint textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 1);
			coreShader2.useTexture("mage_fire");
			glBindVertexArray(coreShader2.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		else if (teste == 2)
		{
			GLuint textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			coreShader2.useTexture("mage_water");
			glBindVertexArray(coreShader2.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		else
		{
			GLuint textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			textureActive = glGetUniformLocation(coreShader2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			coreShader2.useTexture("mage_wind");
			glBindVertexArray(coreShader2.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		if (teste == 1) teste = 2;
		else if (teste == 2) teste = 3;
		else teste = 1; 

		delay(1000);
		
		glfwSwapBuffers( window );
	}
}

void System::Finish()
{
	coreShader.deleteShader();
	glfwTerminate();
}
