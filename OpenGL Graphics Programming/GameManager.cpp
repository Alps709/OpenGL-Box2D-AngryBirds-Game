#include "GameManager.h"
#include "Input.h"
#include "Obj.h"
#include <fmod.h>

GameManager::GameManager()
{
	//Start clock
	m_clock.Initialise();

	//Start audio system
	//AudioInitialise();

	//Create defaut shader
	m_defaultShader = Shader();


	//Set background mesh and texture
	m_backgroundMesh = Mesh(Objects::verticesBackground, Objects::indicesBackground);
	m_backgroundTexture = new Texture("Resources/Images/Grass.png", 0);

	//Texture for the main angry boid
	m_angryBoidTexture = new Texture("Resources/Images/AngryBoid.png", 0);

	//Create 1 background object
	m_backgroundObject = Object(m_backgroundMesh, m_defaultShader, glm::vec2(0.0f, 0.0f));
	m_backgroundObject.SetTexture0(m_backgroundTexture);

	//Create the text objects
	m_menuTitleText = new TextLabel("The Angry Boid Game!", "Resources/Fonts/Arial.ttf", glm::vec2(-625, 200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
	m_menuInstructText = new TextLabel("Press enter to play", "Resources/Fonts/Arial.ttf", glm::vec2(-600, -200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);

	//Create the camera
	//Pass in true to say it is using an orthographic view
	m_camera = std::make_unique <Camera>(true);

	//Box 2D stuff

	//Create the world with the specified gravity
	m_World = std::make_unique<b2World>(m_gravity);

	//Make the empty ground body used for the mousejoint
	b2BodyDef bodyDef;
	m_groundBody = m_World->CreateBody(&bodyDef);

	// Make the screen borders
	CreateScreenBorderWalls();
	
	//Create 10 boxes in the level
	for (int i = 0; i < 10; i++)
	{
		PhysicsBox tempBox = PhysicsBox(m_World.get(), glm::vec2(200.0f, -inputManager.HSCREEN_HEIGHT  + 75.0f * i), glm::vec2(50.0f, 50.0f), 10.0f);
		tempBox.SetTexture0(m_backgroundTexture);
		m_physicsBoxes.push_back(tempBox);
	}

	//Create 1 angry boid (the boid is using a generic PhysicsCircle class for now, I will make a proper angry bird class for the final submission)
	PhysicsCircle tempCircle = PhysicsCircle(m_World.get(), glm::vec2(-450.0f, -100.0f), 25.0f, 25.0f);
	tempCircle.SetTexture0(m_angryBoidTexture);
	m_physicsCircles.push_back(tempCircle);
}

GameManager::~GameManager()
{
	//Delete all the heap allocated objects and clean up others
	m_selectedBoid = nullptr;
	delete m_menuTitleText;
	delete m_menuInstructText;
	delete m_angryBoidTexture;
	delete m_backgroundTexture;
}

//Disable the audio for now
//void GameManager::AudioInitialise()
//{
//
//	FMOD_RESULT result;
//	//Initialise the m_audioSystem
//	result = FMOD::System_Create(&m_audioSystem);
//	if (result != FMOD_OK)
//	{
//		std::cout << "Audio system failed to initialise!";
//		return;
//	}
//
//	result = m_audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
//	if (result != FMOD_OK)
//	{
//		std::cout << "Audio system failed to initialise!";
//	}
//	else
//	{
//		result = m_audioSystem->createSound("Resources/Audio/yeat.wav", FMOD_DEFAULT, 0, &m_yeatSound);
//		if (result != FMOD_OK)
//		{
//			std::cout << "Failed to load sound: yeat.wav" << std::endl;
//		}
//
//		result = m_audioSystem->createSound("Resources/Audio/shoop.wav", FMOD_DEFAULT, 0, &m_shoopSound);
//		if (result != FMOD_OK)
//		{
//			std::cout << "Failed to load sound: shoop.wav" << std::endl;
//		}
//
//		//Create background music
//		//result = m_audioSystem->createSound("Resources/Audio/Jet Set Run.mp3", FMOD_LOOP_NORMAL, 0, &m_trackBackground);
//		//if (result != FMOD_OK)
//		//{
//		//	std::cout << "Failed to load sound: Jet Set Run.mp3" << std::endl;
//		//}
//		//else
//		//{
//		//	//Created sound
//		//	//Start playing background music
//		//	const FMOD_RESULT play = m_audioSystem->playSound(m_trackBackground, 0, false, 0);
//		//	if (play != FMOD_OK)
//		//	{
//		//		std::cout << "Failed to play background track: You Say Run.mp3" << std::endl;
//		//	}
//		//}
//	}
//}

void GameManager::ProcessInput()
{
	//Enter key is pressed
	if (inputManager.KeyState[13] == INPUT_DOWN || inputManager.KeyState[13] == INPUT_DOWN_FIRST)
	{
		//Start game from menu by pressing the enter key
		if (m_gameState == GAME_MENU)
		{
			//Set game state to play
			m_gameState = GAME_PLAY;
		}
	}

	//'O' key is pressed
	if (inputManager.KeyState['o'] == INPUT_DOWN_FIRST || inputManager.KeyState['O'] == INPUT_DOWN_FIRST)
	{
		//Turn wireframe mode on/off
		WireframeRenderMode = !WireframeRenderMode;

		if (WireframeRenderMode)
		{
			GLCall(glPolygonMode(GL_FRONT, GL_LINE));
		}
		else
		{
			GLCall(glPolygonMode(GL_FRONT, GL_FILL));
		}
	}

	//Handle mouse input
	//Left mouse button is pressed
	if (inputManager.MouseState[0] == INPUT_DOWN_FIRST)
	{
		//Update the mouse clicked state
		m_leftMBDown = true;

		//Save where it was clicked
		m_leftMouseDownPos = glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY);

		//Debug logging
		//std::cout << "Mouse button 1 was clicked!" << std::endl;
		//std::cout << "Left mouse button start click pos: " << m_leftMouseDownPos.x << ", " << m_leftMouseDownPos.y << std::endl;
	}

	//Left mouse button is released
	if (inputManager.MouseState[0] == INPUT_UP_FIRST)
	{
		//Update mouse unclicked state
		m_leftMBDown = false;

		//Save where it was let go
		m_leftMouseUpPos = glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY);

		//Debug logging
		//std::cout << "Mouse button 1 was unclicked!" << std::endl;
		//std::cout << "Left mouse button end click pos: " << m_leftMouseUpPos.x << ", " << m_leftMouseUpPos.y << std::endl;
	}
}

void GameManager::CheckMouseCollisions()
{
	//If the left mouse button was clicked on this frame
	if (inputManager.MouseState[0] == INPUT_DOWN_FIRST)
	{
		for (auto& angryBoid : m_physicsCircles)
		{
			//Get the position of the angry boid
			glm::vec2 angryBoidPos = Math::Box2DtoVec2(angryBoid.GetBody()->GetPosition());
			float angryBoidRadius = angryBoid.GetRadius();

			//Check if the mouse is colliding with the angry boid
			if (float(glm::distance(angryBoidPos, m_leftMouseDownPos) - angryBoidRadius) < 0.0f)
			{
				//The player clicked on the angry boid
				std::cout << "Colliding with angry boid!" << std::endl;

				//Set the currently selected boid to the angry boid
				m_selectedBoid = &angryBoid;
				b2Body* angryBoidBody = angryBoid.GetBody();
				
				//Create the mouse joint
				b2MouseJointDef md;
				md.bodyA = m_groundBody;
				md.bodyB = angryBoidBody;

				//The target is where the mousejoint will move the angry boid
				//Target is the mouse position
				md.target = Math::Vec2toBox2D(glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY));
				md.maxForce = 1000.0f * angryBoidBody->GetMass();
				m_mouseJoint = (b2MouseJoint*)m_World->CreateJoint(&md);
				angryBoidBody->SetAwake(true);
			}
		}
	}

	//If the mouse has been moved while the mouse joint exists
	if (m_leftMBDown && m_mouseJoint)
	{
		//Update the target to the new mouse pos
		b2Vec2 newPos = Math::Vec2toBox2D(glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY));
		m_mouseJoint->SetTarget(newPos);
	}

	//If the mouse has been released and the mouse joint exists
	if (!m_leftMBDown && m_mouseJoint)
	{
		m_World->DestroyJoint(m_mouseJoint);
		m_mouseJoint = nullptr;

		//Make the angry boid get shot towards where it was first clicked on
		b2Body* boidBody = m_selectedBoid->GetBody();

		//Calculate the force vector
		glm::vec2 forceVec = m_leftMouseDownPos - m_leftMouseUpPos;
		forceVec = glm::normalize(forceVec) * boidBody->GetMass() * 20000.0f;

		//Apply the force to the angry boid so that it gets flung when the mouse is let go
		boidBody->ApplyForce(Math::Vec2toBox2D(forceVec), boidBody->GetPosition(), true);
		boidBody->SetGravityScale(1.0f);

		m_selectedBoid = nullptr;
	}

	//Test physics - uncommenting this will allow you to have the boxes fly towards the mouse
	/*if (m_leftMBDown)
	{
		for (auto& pBox : m_physicsBoxes)
		{
			glm::vec2 mousePos = glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY);
			auto boxPos = Math::Box2DtoVec2(pBox.GetBody()->GetPosition());
			glm::vec2 vecToMouseClick = mousePos - boxPos;

			vecToMouseClick = glm::normalize(vecToMouseClick) * 1000.0f;
			
			pBox.GetBody()->ApplyForce(Math::Vec2toBox2D(vecToMouseClick), pBox.GetBody()->GetPosition(), true);
		}
	}*/
}

void GameManager::Update(int _mousePosX, int _mousePosY)
{
	//Update clock
	m_clock.Process();
	ProcessInput();

	//Checks for collision of mouse click with collision of the angry boid
	CheckMouseCollisions();

	if (m_gameState == GAME_PLAY)
	{
		for (auto& pBox : m_physicsBoxes)
		{
			glm::vec2 boxPos = Math::Box2DtoVec2(pBox.GetBody()->GetPosition());
		    glm::vec2 boxSize = pBox.GetSize();
			pBox.SetPRS(boxPos.x, boxPos.y, glm::degrees(pBox.GetBody()->GetAngle()), boxSize.x, boxSize.y);
		}

		for (auto& pCircle : m_physicsCircles)
		{
			glm::vec2 circlePos = Math::Box2DtoVec2(pCircle.GetBody()->GetPosition());
			float circleRadius = pCircle.GetRadius();
			pCircle.SetPRS(circlePos.x, circlePos.y, glm::degrees(pCircle.GetBody()->GetAngle()), circleRadius * 2.0f, circleRadius * 2.0f);
		}

		//Update physics simulation only during play
		m_World->Step(1.0f / 60.0f, 12, 12);
	}

	//Update sounds
	//m_audioSystem->update();

	//Tell glut to call the render function again
	glutPostRedisplay();
}

void GameManager::Render()
{
	//Clear the screen before every frame
	Clear();

	//Draw background
	//m_backgroundObject.Render(*m_camera);

	if (m_gameState == GAME_MENU)
	{
		m_menuTitleText->Render();
		m_menuInstructText->Render();
	}
	else if (m_gameState == GAME_PLAY)
	{
		for (PhysicsBox& pBox : m_physicsBoxes)
		{
			pBox.Render(*m_camera);
		}

		for (PhysicsCircle& pCircle : m_physicsCircles)
		{
			pCircle.Render(*m_camera);
		}

		//m_boidStateText->Render();
		//m_containmentStateText->Render();
	}

	glutSwapBuffers();
	u_frameNum++;
}

void GameManager::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void GameManager::CreateScreenBorderWalls()
{
	//Ground border
	b2Vec2 tempPos = Math::Vec2toBox2D(glm::vec2(0.0f, -inputManager.SCREEN_HEIGHT));
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(tempPos.x, tempPos.y);
	b2Body* groundBody = m_World->CreateBody(&groundBodyDef);
	// Make the ground fixture
	auto tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.HSCREEN_WIDTH, inputManager.HSCREEN_HEIGHT));
	b2PolygonShape groundBox;
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Left wall border
	tempPos = Math::Vec2toBox2D(glm::vec2(-inputManager.SCREEN_WIDTH, 00.0f));
	b2BodyDef leftWallBody;
	leftWallBody.position.Set(tempPos.x, tempPos.y);
	groundBody = m_World->CreateBody(&leftWallBody);
	// Make the fixture
	tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.HSCREEN_WIDTH, inputManager.HSCREEN_HEIGHT));
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Right wall border
	tempPos = Math::Vec2toBox2D(glm::vec2(inputManager.SCREEN_WIDTH, 0.0f));
	b2BodyDef rightWallBody;
	rightWallBody.position.Set(tempPos.x, tempPos.y);
	groundBody = m_World->CreateBody(&rightWallBody);
	// Make the fixture
	tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.HSCREEN_WIDTH, inputManager.HSCREEN_HEIGHT));
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);


	//Ground border
	tempPos = Math::Vec2toBox2D(glm::vec2(0.0f, inputManager.SCREEN_HEIGHT));
	b2BodyDef roofBodyDef;
	roofBodyDef.position.Set(tempPos.x, tempPos.y);
	groundBody = m_World->CreateBody(&roofBodyDef);
	// Make the ground fixture
	tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.HSCREEN_WIDTH, inputManager.HSCREEN_HEIGHT));
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);
}
