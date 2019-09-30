#include "System.h"

System::System()
{
}


System::~System()
{
}

bool System::checkCollision(vec4 pVec1, float z1, vec4 pVec2, float z2)
{
	float x1Left  = pVec1.x;
	float y1Left  = pVec1.y;
	float x1Right = pVec1.z;
	float y1Right = pVec1.w;

	float x2Left  = pVec2.x;
	float y2Left  = pVec2.y;
	float x2Right = pVec2.z;
	float y2Right = pVec2.w;
	
	bool blnXResultleft =  (x2Left >= x1Left & x2Left <= x1Right);
	bool blnXResultright = (x2Right >= x1Left & x2Right <= x1Right);

	bool blnYResultleft =  (y2Left  >= y1Left & y2Left  <= y1Right);
	bool blnYResultright = (y2Right >= y1Left & y2Right <= y1Right);

	roundFloat(z1);
	roundFloat(z2);
	float deltaZ = (z1 - z2);
	if (deltaZ < 0) deltaZ *= -1;
	roundFloat(deltaZ);
	bool blnZResult = (deltaZ <= 0.2) ? true: false ;

	if (((blnXResultleft && blnYResultleft) || (blnXResultright && blnYResultright)) && blnZResult)
		return true;
	else
		return false;
}

void System::delay(int milliSeconds)
{
	clock_t start_time = clock();
	while (clock() < start_time + milliSeconds);
};

bool System::timer() 
{
	this->mDblT2 = clock();
	this->mDblDelta = mDblT2 - mDblT1;
	bool blnVar = false;

	if (mDblDelta >= FPS_MIN) {
		blnVar = true;
		mDblT1 = clock();
	}
	else {
		blnVar = false;
	}
	return blnVar;
}

bool System::timerToSpell()
{
	this->mDblT2 = clock();
	this->mDblDelta = mDblT2 - mDblT1;
	bool blnVar = false;

	if (mDblDelta >= FPS_MIN/4) {
		blnVar = true;
		mDblT1 = clock();
	}
	else {
		blnVar = false;
	}
	return blnVar;
}

void System::changeCoords(double offSetPosSpellX, double offSetPosSpellY, double offSetPosSpellX2, double offSetPosSpellY2,
						  double offSetPosWolfX, double offSetPosWolfY, double offSetPosMageX, double offSetPosMageY)
{
	mVecMgSpell.x = offSetPosSpellX;
	mVecMgSpell.y = offSetPosSpellY;
	mVecMgSpell.z = offSetPosSpellX + mgSpellWidth;
	mVecMgSpell.w = offSetPosSpellY + mgSpellHeigth;

	mVecMgSpell2.x = offSetPosSpellX2;
	mVecMgSpell2.y = offSetPosSpellY2;
	mVecMgSpell2.z = offSetPosSpellX2 + mgSpellWidth;
	mVecMgSpell2.w = offSetPosSpellY2 + mgSpellHeigth;

	mVecWolfCoords.x = offSetPosWolfX;
	mVecWolfCoords.y = offSetPosWolfY;
	mVecWolfCoords.z = offSetPosWolfX + wolfWidth;
	mVecWolfCoords.w = offSetPosWolfY + wolfHeigth;

	mVecMageCoords.x = offSetPosMageX;
	mVecMageCoords.y = offSetPosMageY;
	mVecMageCoords.z = offSetPosMageX + mageWidth;
	mVecMageCoords.w = offSetPosMageY + mageHeigth;
}

void System::createVecCoords(vec4& pVecToCreate, float pFltPosX, float pFltPosY, float pFltTamX, float pFltTamY)
{
	float fltX1 = pFltPosX;
	float fltY1 = pFltPosY;
	float fltX2 = pFltPosX + pFltTamX;
	float fltY2 = pFltPosY + pFltTamY;
	pVecToCreate = vec4(fltX1, fltY1, fltX2, fltY2);
}

int System::GLFWInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "MageGame", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{

	glEnable(GL_BLEND);	// Enables blending ( glBlendFunc )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	sky = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",		2.00f, 2.00f,  0.00f, -1.00f, -0.80f);
	clouds = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",		2.00f, 0.33f, -0.01f, -1.00f, 0.65f);
	mountains = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",  3.42f, 2.00f, -0.02f, -1.00f, -0.70f);
	fence = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",		3.42f, 2.00f, -0.03f);
	grass = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag",		3.42f, 2.00f, -0.04f);

	//lifebar
	float lifeWidth = 0.1;
	float lifeHeight = 0.025f;
	float lifeOffSet = 0.25;
	lifeBar = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", lifeWidth, lifeHeight, -1, -1, 2- lifeHeight, lifeOffSet);

	float manaWidth = 0.1;
	float manaHeight = 0.126;
	float manaOffSet = 0.39;
	manaBar = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", manaWidth, manaHeight, -1, -1, 2 - lifeHeight - manaHeight, manaOffSet);
	
	//mage creation
	mageWidth = 0.402f;
	mageHeigth = 0.432f;
	float magePosX = 0.00f;
	float magePosY = rndFloat();
	magePosZ = magePosY;
	float mageOffSetTex = 0.059;
	mage = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", mageWidth, mageHeigth, 0, 0, 0, mageOffSetTex);
	createVecCoords(mVecMageCoords, magePosX, magePosY, mageWidth, mageHeigth);

	//wolf creation
	wolfWidth = 0.5184f;
	wolfHeigth = 0.4968f;
	float wolfPosX = 3.0f;
	float wolfPosY = rndFloat();
	wolfPosZ = wolfPosY;
	float wolfOffSetTex = 0.055f;
	wolf = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", wolfWidth, wolfHeigth, 0, 0, 0, wolfOffSetTex);
	createVecCoords(mVecWolfCoords, wolfPosX, wolfPosY, wolfWidth, wolfHeigth);

	//Spell 1 creation
	mgSpellWidth = 0.3f;
	mgSpellHeigth = 0.3f;
	float mgSpellPosX = magePosX + mageWidth * 0.7f;
	float mgSpellPosY = magePosY + mageHeigth * 0.25f;
	mgSpellPosZ = 0.00f;
	mageSpell = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", mgSpellWidth, mgSpellHeigth, mgSpellPosZ, 0, 0);
	createVecCoords(mVecMgSpell, mgSpellPosX, mgSpellPosY, mgSpellWidth, mgSpellHeigth);

	//Spell2 creation
	mageSpell2 = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag", mgSpellWidth, mgSpellHeigth, mgSpellPosZ, 0, 0);
	createVecCoords(mVecMgSpell, mgSpellPosX, mgSpellPosY, mgSpellWidth, mgSpellHeigth);

	sky.useShader();
	clouds.useShader();
	mountains.useShader();
	fence.useShader();
	grass.useShader();
	mage.useShader();
	wolf.useShader();
	mageSpell.useShader();
	mageSpell2.useShader();

	return EXIT_SUCCESS;
}

void System::Run()
{
	lifeBar.useShader();
	lifeBar.loadTexture("bin/assets/Background/lifeBar.png", "texture1", "life", false);
	if (!lifeBar.bindVAO())
		return;

	manaBar.useShader();
	manaBar.loadTexture("bin/assets/Background/mana.png", "texture1", "mana", false);
	if (!manaBar.bindVAO())
		return;

	//load sky
	sky.useShader();
	sky.loadTexture("bin/assets/Background/sky.png", "texture1", "sky", true);
	if (!sky.bindVAO())
		return;

	//load clouds
	clouds.useShader();
	clouds.loadTexture("bin/assets/Background/clouds.png", "texture1", "clouds", true);
	if (!clouds.bindVAO())
		return;

	//load mountains
	mountains.useShader();
	mountains.loadTexture("bin/assets/Background/mountains.png", "texture1", "mountains", true);
	if (!mountains.bindVAO())
		return;

	//load trees
	fence.useShader();
	fence.loadTexture("bin/assets/Background/fence.png", "texture1", "fence", true);
	if (!fence.bindVAO())
		return;

	//load grass
	grass.useShader();
	grass.loadTexture("bin/assets/Background/grass.png", "texture1", "grass", true);
	if (!grass.bindVAO())
		return;

	//load mages
	mage.useShader();
	mage.loadTexture("bin/assets/Mage/mage_fire.png", "texture1", "mage_fire", false);
	mage.loadTexture("bin/assets/Mage/mage_water.png", "texture2", "mage_water", false);
	mage.loadTexture("bin/assets/Mage/mage_wind.png", "texture3", "mage_wind", false);
	if (!mage.bindVAO())
		return;

	//load wolfes
	wolf.useShader();
	wolf.loadTexture("bin/assets/Wolf/wolf_fire.png", "texture1", "wolf_fire", false);
	wolf.loadTexture("bin/assets/Wolf/wolf_water.png", "texture2", "wolf_water", false);
	wolf.loadTexture("bin/assets/Wolf/wolf_wind.png", "texture3", "wolf_wind", false);
	if (!wolf.bindVAO())
		return;

	//load mage Spell
	mageSpell.useShader();
	mageSpell.loadTexture("bin/assets/Attack/attack_fire.png", "texture1", "attack_fire", false);
	mageSpell.loadTexture("bin/assets/Attack/attack_water.png", "texture2", "attack_water", false);
	mageSpell.loadTexture("bin/assets/Attack/attack_wind.png", "texture3", "attack_wind", false);
	if (!mageSpell.bindVAO())
		return;

	//load mage Spell 2
	mageSpell2.useShader();
	mageSpell2.loadTexture("bin/assets/Attack/attack_fire.png", "texture1", "attack_fire2", false);
	mageSpell2.loadTexture("bin/assets/Attack/attack_water.png", "texture2", "attack_water2", false);
	mageSpell2.loadTexture("bin/assets/Attack/attack_wind.png", "texture3", "attack_wind2", false);
	if (!mageSpell2.bindVAO())
		return;

	//scrolling speed
	double offsetx = 0;
	double ScrollingSpeed = 0.05;

	//mage vars
	int intMageElement = 1;
	double offSetXTexMage = 0;
	double offSetPosMageX = mVecMageCoords.x, offSetPosMageY = mVecMageCoords.y, offSetPosMageZ = magePosZ;
	double mageSpeedX = 0.02f;
	double mageSpeedY = 0.02f;
	float mageSpeedZ  = 0.1f;
	bool blnCastSpell = false;
	int intSpellCycle = 0;

	double offSetXMageAmount = 0.058823529;
	double offSetMageLimit = 0.941176463;

	//Spell vars
	int intSpellElement = 1;
	double offSetPosSpellX = mVecMgSpell.x, offSetPosSpellY = mVecMgSpell.y, offSetPosSpellZ = mgSpellPosZ;
	double spellSpeedX = 0.02;
	double spellSpeedY = 0.02;
	bool blnSpellActive = false;

	//Spell 2 vars
	int intSpellElement2 = 1;
	double offSetPosSpellX2 = mVecMgSpell2.x, offSetPosSpellY2 = mVecMgSpell2.y, offSetPosSpellZ2 = mgSpellPosZ;
	double spellSpeedX2 = 0.02;
	double spellSpeedY2 = 0.02;
	bool blnSpellActive2 = false;
		
	//wolf vars;
	double offSetXTexWolf = 0;
	double offSetPosWolfX = mVecWolfCoords.x , offSetPosWolfY = mVecWolfCoords.y, offSetPosWolfZ = wolfPosZ;
	double wolfSpeedX = mageSpeedX;
	double wolfSpeedY = mageSpeedY;

	double offSetXWolfAmount = 0.055555555;
	double offSetWolfLimit = 0.94444443;

	//cloud vars
	double offsetCloudX = 0;

	//flags
	bool blnBtnPressed = false, blnUpdateTexs = false;

	while (!glfwWindowShouldClose(window)) {
		changeCoords( offSetPosSpellX,  offSetPosSpellY,  offSetPosSpellX2,  offSetPosSpellY2,
					  offSetPosWolfX,  offSetPosWolfY,  offSetPosMageX,  offSetPosMageY);

		blnBtnPressed = false;
		blnUpdateTexs = this-> timer();
		
		glfwPollEvents();
#pragma region Collision
		if (checkCollision(mVecMageCoords, offSetPosMageZ,mVecWolfCoords, offSetPosWolfZ))
		{
			if (blnUpdateTexs) {
				intCount++;
				if (intCount == 3)
					glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
		}

		if (blnSpellActive && checkCollision(mVecWolfCoords, offSetPosWolfZ, mVecMgSpell, offSetPosSpellZ))
		{
			offSetPosWolfZ = 0.0;

			offSetPosWolfX = 6;
			offSetPosWolfZ = rndFloat();
			offSetPosWolfY = offSetPosWolfZ;
			wolfElement = rand() % 3 + 1;
		}

		if (blnSpellActive2 && checkCollision(mVecWolfCoords, offSetPosWolfZ, mVecMgSpell2, offSetPosSpellZ2))
		{
			offSetPosWolfZ = 0.0;

			offSetPosWolfX = 6;
			offSetPosWolfZ = rndFloat();
			offSetPosWolfY = offSetPosWolfZ;
			wolfElement = rand() % 3 + 1;
		}

#pragma endregion

#pragma region Input Handling

		//close window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		//mage forward
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (mVecMageCoords.z < 0.8f)
			{
				//mage scrolling -->
				this->offSetIncrement(offSetPosMageX, mageSpeedX);
				/*this->changeCoords(mVecMageCoords, mageSpeedX, 0.0f);*/
			}
			else
			{
				//scrolling scenario-->
				this->offSetIncrement(offsetx, ScrollingSpeed);
			}

			if (blnUpdateTexs)
			{
				if (offSetXTexMage <= (offSetXMageAmount * 3 - MINUS) || offSetXTexMage >= (offSetXMageAmount * 6 - MINUS))
					offSetXTexMage = offSetXMageAmount * 3;

				else if (offSetXTexMage <= (offSetXMageAmount * 5- MINUS))
					offSetXTexMage += offSetXMageAmount;

				else if (offSetXTexMage >= (offSetXMageAmount * 5 - MINUS))
					offSetXTexMage = offSetXMageAmount * 4;
			}
			blnBtnPressed = true;
		}

		//mage back
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (mVecMageCoords.x > -1.0f)
			{
				//mage scrolling <--
				this->offSetDecrement(offSetPosMageX, mageSpeedX);
				//this->changeCoords(mVecMageCoords, -mageSpeedX, 0.0f);
			}
			else
			{
				//scrolling scenario <--
				this->offSetDecrement(offsetx, ScrollingSpeed);
			}

			if (blnUpdateTexs)
			{
				if (offSetXTexMage >= (offSetXMageAmount * 9 - MINUS) || offSetXTexMage <= (offSetXMageAmount * 5 - MINUS))
					offSetXTexMage = offSetXMageAmount * 8;

				else if (offSetXTexMage >= (offSetXMageAmount * 7 - MINUS))
					offSetXTexMage -= offSetXMageAmount;

				else if (offSetXTexMage <= (offSetXMageAmount * 7 - MINUS))
					offSetXTexMage = offSetXMageAmount * 7;
			}
			blnBtnPressed = true;
		}

		//mage up
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (mVecMageCoords.y < -0.5f)
			{
				//mage scrolling up
				this->offSetIncrement(offSetPosMageY, mageSpeedX);
				if (offSetPosMageZ <= -0.5)
				{
					this->offSetIncrement(offSetPosMageZ, mageSpeedZ);
				}
			}
			blnBtnPressed = true;
		}

		//mage down
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if (mVecMageCoords.y > -1.0f)
			{
				
				//mage scrolling down
				this->offSetDecrement(offSetPosMageY, mageSpeedX);
				if (offSetPosMageZ >= -0.9)
				{
					this->offSetDecrement(offSetPosMageZ, mageSpeedZ);
				}
			}
			blnBtnPressed = true;
		}

		//mage cast spell
		blnUpdateTexs;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
		{
			blnCastSpell = true;
		}

		//state machine to castSpell
		if (blnCastSpell && timerToSpell())
		{
			if (intSpellCycle == 0)
			{
				offSetXTexMage = offSetXMageAmount * 9;
				intSpellCycle = 1;
			}
			else if (intSpellCycle == 1)
			{
				offSetXTexMage = offSetXMageAmount * 10;
				intSpellCycle = 2;
			}
			else if (intSpellCycle == 2)
			{
				offSetXTexMage = offSetXMageAmount * 11;
				intSpellCycle = 3;
			}
			else if (intSpellCycle == 3)
			{
				offSetXTexMage = offSetXMageAmount * 12;
				intSpellCycle = 4;
			}
			else if (intSpellCycle == 4)
			{
				offSetXTexMage = offSetXMageAmount * 13;
				intSpellCycle = 5;
			}
			else if (intSpellCycle == 5)
			{
				offSetXTexMage = offSetXMageAmount * 14;
				intSpellCycle = 6;
			}
			else if (intSpellCycle == 6)
			{
				offSetXTexMage = offSetXMageAmount * 15;
				intSpellCycle = 7;
			}
			else if(intSpellCycle == 7)
			{
				offSetXTexMage = offSetXMageAmount * 16;
				intSpellCycle = 8;

				if (blnSpellActive)
				{
					offSetPosSpellZ2 = -1.0f;
					this->createVecCoords(mVecMgSpell2, 0.0f, 0.0f, 0.0f, 0.0f);
					offSetPosSpellX2 = mVecMageCoords.x + mageWidth * 0.7f;
					offSetPosSpellY2 = mVecMageCoords.y + mageHeigth * 0.25f;
					offSetPosSpellZ2 = offSetPosMageZ;
					blnSpellActive2 = true;
					intSpellElement2 = intMageElement;
				}
				else
				{
					offSetPosSpellZ = -1.0f;
					this->createVecCoords(mVecMgSpell, 0.0f, 0.0f, 0.0f, 0.0f);
					offSetPosSpellX = mVecMageCoords.x + mageWidth * 0.7f;
					offSetPosSpellY = mVecMageCoords.y + mageHeigth * 0.25f;
					offSetPosSpellZ = offSetPosMageZ;
					blnSpellActive = true;
					intSpellElement = intMageElement;
				}				
			}
			else
			{
				offSetXTexMage = 0;
				intSpellCycle = 0;
				blnCastSpell = false;
			}
		}

		//update Spell
		if (blnSpellActive)
		{
			if (mVecMgSpell.x < 2.0f)
			{
				//Spell scrolling -->
				double dblSpellSpeed = mageSpeedX * 1.5f;
				this->offSetIncrement(offSetPosSpellX, dblSpellSpeed);
				//this->changeCoords(mVecMgSpell, dblSpellSpeed, 0.0f);
			}
			else
			{
				this->offSetDecrement(offSetPosSpellX, 2.0f);
				//this->changeCoords(mVecMgSpell, -2.0f, 0.0f);
				offSetPosSpellZ = 0.0f;
				blnSpellActive = false;
				intSpellElement = intMageElement;
			}
		}

		//update Spell 2
		if (blnSpellActive2)
		{
			if (mVecMgSpell2.x < 2.0f)
			{
				//Spell scrolling -->
				double dblSpellSpeed = mageSpeedX * 1.5f;
				this->offSetIncrement(offSetPosSpellX2, dblSpellSpeed);
				//this->changeCoords(mVecMgSpell2, dblSpellSpeed, 0.0f);
			}
			else
			{
				this->offSetDecrement(offSetPosSpellX2, 2.0f);
				//this->changeCoords(mVecMgSpell2, -2.0f, 0.0f);
				offSetPosSpellZ2 = 0.0f;
				blnSpellActive2 = false;
				intSpellElement2 = intMageElement;
			}
		}

		//mage change to fire
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			intMageElement = 1;
			if(!blnSpellActive)
				intSpellElement = intMageElement;
			if (!blnSpellActive2)
				intSpellElement2 = intMageElement;
		}

		//mage change to water
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			intMageElement = 2;
			if (!blnSpellActive)
				intSpellElement = intMageElement;
			if (!blnSpellActive2)
				intSpellElement2 = intMageElement;
		}

		//mage change to wind
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			intMageElement = 3;
			if (!blnSpellActive)
				intSpellElement = intMageElement;
			if (!blnSpellActive2)
				intSpellElement2 = intMageElement;
		}

		//mage in Idle
		if( !blnBtnPressed && blnUpdateTexs && !blnCastSpell)
		{
			offSetXTexMage <= (offSetXMageAmount * 2 - MINUS) ? offSetXTexMage += offSetXMageAmount : offSetXTexMage = 0;
		}

		//reset position of wolf
		if (offSetPosWolfX <= -3.00f)
		{
			this->offSetIncrement(offSetPosWolfX, 6);
			offSetPosWolfZ = rndFloat();
			offSetPosWolfY = offSetPosWolfZ;

			wolfElement = rand() % 3 + 1;
		}

		//wolf scrolling <--
		this->offSetDecrement(offSetPosWolfX, wolfSpeedX);
		//this->changeCoords(mVecWolfCoords, -ScrollingSpeed, 0.0f);

#pragma endregion

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//sky update
		sky.useShader();
		sky.useTexture("sky");
		glBindVertexArray(sky.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		manaBar.useShader();
		manaBar.useTexture("mana");
		glBindVertexArray(lifeBar.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//sky update
		lifeBar.useShader();
		lifeBar.useTexture("life");
		glBindVertexArray(lifeBar.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//clouds update
		this->offSetDecrement(offsetCloudX, 0.0001);
		clouds.useShader();
		glUniform1f(glGetUniformLocation(clouds.getProgramId(), "offsetx"), offsetCloudX);
		clouds.useTexture("clouds");
		glBindVertexArray(clouds.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//mountains update
		mountains.useShader();
		glUniform1f(glGetUniformLocation(mountains.getProgramId(), "offsetx"), offsetx / 600);
		mountains.useTexture("mountains");
		glBindVertexArray(mountains.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//fence update
		fence.useShader();
		glUniform1f(glGetUniformLocation(fence.getProgramId(), "offsetx"), offsetx / 20);
		fence.useTexture("fence");
		glBindVertexArray(fence.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//grass update
		grass.useShader();
		glUniform1f(glGetUniformLocation(grass.getProgramId(), "offsetx"), offsetx / 20);
		grass.useTexture("grass");
		glBindVertexArray(grass.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//Mage update
		mage.useShader();
		if (intMageElement == 1) {
			GLuint textureActive = glGetUniformLocation(mage.getProgramId(), "textureActive");
			glUniform1i(textureActive, 1);
			mage.useTexture("mage_fire");
		}
		else if (intMageElement == 2)
		{
			GLuint textureActive = glGetUniformLocation(mage.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			mage.useTexture("mage_water");
		}
		else
		{
			GLuint textureActive = glGetUniformLocation(mage.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			mage.useTexture("mage_wind");
		}
		glUniform1f(glGetUniformLocation(mage.getProgramId(), "offsetx"), offSetXTexMage);
		glUniform1f(glGetUniformLocation(mage.getProgramId(), "offsetVSy"), offSetPosMageY);
		glUniform1f(glGetUniformLocation(mage.getProgramId(), "offsetVSx"), offSetPosMageX);
		glUniform1f(glGetUniformLocation(mage.getProgramId(), "offsetVSz"), offSetPosMageZ);
		glBindVertexArray(mage.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//Wolf update
		wolf.useShader();
		if (wolfElement == 1) {
			GLuint textureActive = glGetUniformLocation(wolf.getProgramId(), "textureActive");
			glUniform1i(textureActive, 1);
			wolf.useTexture("wolf_fire");
		}
		else if (wolfElement == 2)
		{
			GLuint textureActive = glGetUniformLocation(wolf.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			wolf.useTexture("wolf_water");
		}
		else
		{
			GLuint textureActive = glGetUniformLocation(wolf.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			wolf.useTexture("wolf_wind");
		}
		glUniform1f(glGetUniformLocation(wolf.getProgramId(), "offsetx"), offSetXTexWolf);
		glUniform1f(glGetUniformLocation(wolf.getProgramId(), "offsetVSx"), offSetPosWolfX);
		glUniform1f(glGetUniformLocation(wolf.getProgramId(), "offsetVSy"), offSetPosWolfY);
		glUniform1f(glGetUniformLocation(wolf.getProgramId(), "offsetVSz"), offSetPosWolfZ);
		glBindVertexArray(wolf.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);		

		//Spell update
		mageSpell.useShader();
		if (intSpellElement == 1) {
			GLuint textureActive = glGetUniformLocation(mageSpell.getProgramId(), "textureActive");
			glUniform1i(textureActive, 1);
			mageSpell.useTexture("attack_fire");
		}
		else if (intSpellElement == 2)
		{
			GLuint textureActive = glGetUniformLocation(mageSpell.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			mageSpell.useTexture("attack_water");
		}
		else
		{
			GLuint textureActive = glGetUniformLocation(mageSpell.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			mageSpell.useTexture("attack_wind");
		}
		glUniform1f(glGetUniformLocation(mageSpell.getProgramId(), "offsetVSy"), offSetPosSpellY);
		glUniform1f(glGetUniformLocation(mageSpell.getProgramId(), "offsetVSx"), offSetPosSpellX);
		glUniform1f(glGetUniformLocation(mageSpell.getProgramId(), "offsetVSz"), offSetPosSpellZ);
		glBindVertexArray(mageSpell.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//Spell 2 update
		mageSpell2.useShader();
		if (intSpellElement2 == 1) {
			GLuint textureActive = glGetUniformLocation(mageSpell2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 1);
			mageSpell2.useTexture("attack_fire2");
		}
		else if (intSpellElement2 == 2)
		{
			GLuint textureActive = glGetUniformLocation(mageSpell2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 2);
			mageSpell2.useTexture("attack_water2");
		}
		else
		{
			GLuint textureActive = glGetUniformLocation(mageSpell2.getProgramId(), "textureActive");
			glUniform1i(textureActive, 3);
			mageSpell2.useTexture("attack_wind2");
		}
		glUniform1f(glGetUniformLocation(mageSpell2.getProgramId(), "offsetVSy"), offSetPosSpellY2);
		glUniform1f(glGetUniformLocation(mageSpell2.getProgramId(), "offsetVSx"), offSetPosSpellX2);
		glUniform1f(glGetUniformLocation(mageSpell2.getProgramId(), "offsetVSz"), offSetPosSpellZ2);
		glBindVertexArray(mageSpell2.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		if (blnUpdateTexs) {
			if (offSetXTexWolf >= offSetWolfLimit) { offSetXTexWolf = offSetXWolfAmount; }
			else offSetXTexWolf += offSetXWolfAmount;
		}
		glfwSwapBuffers(window);
	}
}

void System::Finish()
{
	mountains.deleteShader();
	mage.deleteShader();
	sky.deleteShader();
	clouds.deleteShader();
	grass.deleteShader();
	wolf.deleteShader();
	mageSpell.deleteShader();
	mageSpell2.deleteShader();
	glfwTerminate();
}

void System::offSetIncrement(double& pDblVar, double pDblAmountToInc)
{
	pDblVar >= INT32_MAX ? pDblVar = 0.0 : pDblVar += pDblAmountToInc;
}

void System::offSetDecrement(double& pDblVar, double pDblAmountToDec)
{
	pDblVar <= INT32_MIN ? pDblVar = 0.0 : pDblVar -= pDblAmountToDec;
}
