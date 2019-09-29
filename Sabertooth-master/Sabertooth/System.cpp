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
	sky = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",        2.00f, 2.00f,  0.00f, -1.00f, -0.80f);
	clouds = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",     2.00f, 0.33f, -0.10f, -1.00f,  0.65f);
	mountains = Shader( "Shaders/Core/core.vert", "Shaders/Core/core.frag", 3.42f, 2.00f, -0.20f, -1.00f, -0.70f);
	grass = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",      3.42f, 2.00f, -0.90f);
	mage = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",       0.402f, 0.432f, -1.00f,  0.00f,  -0.75f, 0.059, 1);
	
	

	mountains.useShader();
	mage.useShader();
	sky.useShader();
	clouds.useShader();
	grass.useShader();

	return EXIT_SUCCESS;
}

void System::Run()
{
	//load grass
	grass.useShader();
	grass.loadTexture("bin/assets/Background/grass.png", "texture1", "grass", true);
	if (!grass.bindVAO())
		return;

	//load clouds
	clouds.useShader();
	clouds.loadTexture("bin/assets/Background/clouds.png", "texture1", "clouds", true);
	if (!clouds.bindVAO())
		return;

	//load sky
	sky.useShader();
	sky.loadTexture("bin/assets/Background/sky.png", "texture1", "sky", true);
	if (!sky.bindVAO())
		return;

	//load mountains
	mountains.useShader();
	mountains.loadTexture( "bin/assets/Background/mountains2.png", "texture1", "mountains", true );
	if (!mountains.bindVAO())
		return;
	
	//load mages
	mage.useShader();
	mage.loadTexture("bin/assets/Mage/mage_fire2.png", "texture1", "mage_fire", false);
	mage.loadTexture("bin/assets/Mage/mage_water2.png", "texture2", "mage_water", false);
	mage.loadTexture("bin/assets/Mage/mage_wind2.png", "texture3", "mage_wind", false);
	if (!mage.bindVAO())
		return;
	
	int teste = 1;
	float offsetx = 0;

	double offSetXMage = 0;

	while ( !glfwWindowShouldClose( window ) ) {

		glfwPollEvents();

#pragma region Input Handling

		if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
			glfwSetWindowShouldClose( window, GLFW_TRUE );
		}

#pragma endregion

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


		//sky update
		sky.useShader();
		sky.useTexture("sky");
		glBindVertexArray(sky.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//grass update
		grass.useShader();
		glUniform1f(glGetUniformLocation(grass.getProgramId(), "offsetx"), offsetx);
		grass.useTexture("grass");
		glBindVertexArray(grass.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//clouds update
		clouds.useShader();
		glUniform1f(glGetUniformLocation(clouds.getProgramId(), "offsetx"), offsetx/100);
		clouds.useTexture("clouds");
		glBindVertexArray(clouds.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//background update
		mountains.useShader();
		glUniform1f(glGetUniformLocation(mountains.getProgramId(), "offsetx"), offsetx/60);
		mountains.useTexture( "mountains" );
		glBindVertexArray(mountains.getVAO());
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray( 0 );

		//Mage update
		mage.useShader();
		
		/*if (teste == 1) {*/
			GLuint textureActive = glGetUniformLocation(mage.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			glUniform1f(glGetUniformLocation(mage.getProgramId(), "offsetx"), offSetXMage);
			mage.useTexture("mage_wind");
			glBindVertexArray(mage.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		//}

		/*else if (teste == 2)
		{
			GLuint textureActive = glGetUniformLocation(mage.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			mage.useTexture("mage_water");
			glBindVertexArray(mage.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		else
		{
			GLuint textureActive = glGetUniformLocation(mage.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			mage.useTexture("mage_wind");
			glBindVertexArray(mage.getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}*/

		if (teste == 1) teste = 2;
		else if (teste == 2) teste = 3;
		else teste = 1; 

		if (offSetXMage >= 0.941176463) { offSetXMage = 0.058823529; }
		else offSetXMage += 0.058823529;

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

		delay(120);
		
		glfwSwapBuffers( window );
	}
}

void System::Finish()
{
	mountains.deleteShader();
	mage.deleteShader();
	sky.deleteShader();
	clouds.deleteShader();
	grass.deleteShader();
	glfwTerminate();
}
