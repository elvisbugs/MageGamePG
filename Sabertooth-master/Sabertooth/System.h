#ifndef SYSTEM_H
#define SYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"

#include <vector>

#define MINUS		0.000000001
#define SUPER_MINUS 0.000000000000000000000000000000001

using namespace std;

using namespace glm;

class System
{
private:
	// Screen
	const GLint WIDTH = 800, HEIGHT = 600;
	int screenWidth, screenHeight;

	double mDblT1 = clock();
	double mDblT2 = 0;
	double mDblDelta = 0;

	bool timer();
	bool timerToSpell();

	double FPS_MIN = 180;

	vec4 mVecMageCoords;
	vec4 mVecWolfCoords;
	vec4 mVecMgSpell;
	vec4 mVecMgSpell2;

	float mageWidth = 0;
	float mageHeigth = 0;

	float wolfWidth  = 0;
	float wolfHeigth = 0;

	float mgSpellWidth = 0;
	float mgSpellHeigth = 0;

	void changeCoords(double offSetPosSpellX, double offSetPosSpellY, double offSetPosSpellX2, double offSetPosSpellY2,
					  double offSetPosWolfX, double offSetPosWolfY, double offSetPosMageX, double offSetPosMageY);

	void createVecCoords(vec4 &pVecToCreate, float pFltPosX, float pFltPosY, float pFltTamX, float pFltTamY);

	void offSetIncrement(double& pDblVar, double pDblAmountToInc);
	void offSetDecrement(double& pDblVar, double pDblAmountToDec);

	bool checkCollision(vec4 pVec1, vec4 pVec2);

public:
	GLFWwindow* window;

	Shader mountains;
	Shader mage;
	Shader sky;
	Shader clouds;
	Shader grass;
	Shader fence;
	Shader wolf;
	Shader mageSpell;
	Shader mageSpell2;
	
	System();
	~System();

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();

	void Run();

	void Finish();

	void delay(int milliSeconds);

};

#endif