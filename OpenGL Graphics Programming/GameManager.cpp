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

	
	//Texture for the main angry boid
	m_angryBoidTexture = new Texture("Resources/Images/RedBird.png", 0);

	//Texture for box
	m_boxTexture = new Texture("Resources/Images/Grass.png", 0);
	//Texture for damaged box
	m_damagedBoxTexture = new Texture("Resources/Images/GrassDamaged.png", 0);


	//Texture for the piggie
	m_piggieTexture = new Texture("Resources/Images/Piggie.png", 0);
	//Texture for the dead piggie
	m_piggieTexture1 = new Texture("Resources/Images/PiggieHurt.png", 0);

	//Create the text objects
	m_menuTitleText = std::make_shared<TextLabel>("The Angry Boid Game!", "Resources/Fonts/Arial.ttf", glm::vec2(-625, 200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
	m_menuInstructText = std::make_shared<TextLabel>("Press enter to play", "Resources/Fonts/Arial.ttf", glm::vec2(-600, -200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
	m_gameScoreText = std::make_shared<TextLabel>("Score: ", "Resources/Fonts/Arial.ttf", glm::vec2(-inputManager.GL_HSCREEN_WIDTH + 40.0f, inputManager.GL_HSCREEN_HEIGHT - 40.0f), glm::vec3(0.0f, 1.0f, 1.0f), 1.0f);
	m_gameOverTitleText = std::make_shared<TextLabel>("Game over!", "Resources/Fonts/Arial.ttf", glm::vec2(-625, 200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
	m_gameOverInstructText = std::make_shared<TextLabel>("Press enter to play again!", "Resources/Fonts/Arial.ttf", glm::vec2(-625, -200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);

	//Create the camera
	//Pass in true to say it is using an orthographic view
	m_camera = std::make_unique <Camera>(true);

	//Box 2D stuff

	//Create the world with the specified gravity
	m_World = std::make_unique<b2World>(m_gravity);

	m_World->SetContactListener(&m_collisionListener);

	//Make the empty ground body used for the mousejoint
	b2BodyDef bodyDef;
	m_groundBody = m_World->CreateBody(&bodyDef);

	// Make the screen borders
	CreateScreenBorderWalls();
	
	//Create 15 boxes in the level (the first 9 of them are destructable)
	for (int i = 0; i < 15; i++)
	{
		glm::vec2 size = glm::vec2(40.0f, 40.0f);
		if (i >= 10) size = glm::vec2(50.0f, 50.0f);

		m_physicsBoxes.push_back(std::move(std::make_shared<PhysicsBox>(m_World.get(), glm::vec2(200.0f, -inputManager.GL_HSCREEN_HEIGHT  + 40.0f + (((i >= 10)? 45 : 40) * i)), size, 10.0f)));
		m_physicsBoxes.back()->SetTexture0(m_boxTexture);
		m_physicsBoxes.back()->SetTexture1(m_damagedBoxTexture);

		if (i < 10)
		{
			m_physicsBoxes.back()->SetDestructable(true);
		}

		m_physicsBoxes.back()->SetUserData();
	}

	//Create 1 angry boid (the boid is using a generic PhysicsCircle class for now, I will make a proper angry bird class for the final submission)
	m_angryBoids.push_back(std::move(std::make_shared<AngryBoid>(m_World.get(), m_boidFirePos, 25.0f, 25.0f)));
	m_angryBoids.back()->SetTexture0(m_angryBoidTexture);
	m_angryBoids.back()->SetFireable(true);

	for (int i = 0; i < 3; i++)
	{
		//Create 1 angry boid (the boid is using a generic PhysicsCircle class for now, I will make a proper angry bird class for the final submission)
		m_angryBoids.push_back(std::move(std::make_shared<AngryBoid>(m_World.get(), glm::vec2(-inputManager.GL_HSCREEN_WIDTH + 100 + (100 * i), -inputManager.GL_HSCREEN_HEIGHT + 50), 25.0f, 25.0f)));
		m_angryBoids.back()->SetTexture0(m_angryBoidTexture);
		m_angryBoids.back()->SetFireable(false);
	}

	//Create 1 piggie on the seesaw
	m_piggies.push_back(std::move(std::make_shared<Piggie>(m_World.get(), glm::vec2(400.0f, 40.0f), 25.0f, 25.0f)));
	m_piggies.back()->SetTexture0(m_piggieTexture);
	m_piggies.back()->SetTexture1(m_piggieTexture1);
	m_piggies.back()->SetUserData();

	for (int i = 0; i < 3; i++)
	{
		//Create 3 piggies
		m_piggies.push_back(std::move(std::make_shared<Piggie>(m_World.get(), glm::vec2(inputManager.GL_HSCREEN_WIDTH - 50.0f - (150.0f * i), -inputManager.GL_HSCREEN_HEIGHT + 50), 25.0f, 25.0f)));
		m_piggies.back()->SetTexture0(m_piggieTexture);
		m_piggies.back()->SetTexture1(m_piggieTexture1);
		m_piggies.back()->SetUserData();
	}

	//Create 1 seesaw joint
	m_physicsSeesaw = std::make_shared<PhysicsSeesaw>(m_World.get(), glm::vec2(400.0f, 0.0f), glm::vec2(300.0f, 20.0f), 10.0f);
	m_physicsSeesaw->SetTexture0(m_boxTexture);
}

GameManager::~GameManager()
{
	//Delete all the heap allocated objects and clean up others
	//Clear the entities in the level
	m_selectedBoid = nullptr;
	m_physicsBoxes.clear();
	m_angryBoids.clear();

	delete m_angryBoidTexture;
	delete m_boxTexture;
	delete m_damagedBoxTexture;
	delete m_piggieTexture;
	delete m_piggieTexture1;
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

		if (m_gameState == GAME_OVER)
		{
			m_gameState = GAME_PLAY;
			Reset();
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

	if (inputManager.KeyState['r'] == INPUT_DOWN_FIRST)
	{
		Reset();
	}
}

void GameManager::Update(int _mousePosX, int _mousePosY)
{
	//Update clock
	m_clock.Process();
	ProcessInput();

	//Checks for collision of mouse click with collision of the angry boid
	CheckMouseToBoidCollisions();

	if (m_gameState == GAME_PLAY)
	{
		glm::vec2 seesawPos = Math::Box2DtoVec2(m_physicsSeesaw->GetBody()->GetPosition());
		glm::vec2 seesawSize = m_physicsSeesaw->GetSize();
		m_physicsSeesaw->SetPRS(seesawPos.x, seesawPos.y, glm::degrees(m_physicsSeesaw->GetBody()->GetAngle()), seesawSize.x, seesawSize.y);

		for (auto& pBox : m_physicsBoxes)
		{
			glm::vec2 boxPos = Math::Box2DtoVec2(pBox->GetBody()->GetPosition());
		    glm::vec2 boxSize = pBox->GetSize();
			pBox->SetPRS(boxPos.x, boxPos.y, glm::degrees(pBox->GetBody()->GetAngle()), boxSize.x, boxSize.y);
		}

		for (auto& pBoid : m_angryBoids)
		{
			glm::vec2 boidPos = Math::Box2DtoVec2(pBoid->GetBody()->GetPosition());
			float boidRadius = pBoid->GetRadius();
			pBoid->SetPRS(boidPos.x, boidPos.y, glm::degrees(pBoid->GetBody()->GetAngle()), boidRadius * 2.0f, boidRadius * 2.0f);
		}

		for (auto& pPiggie : m_piggies)
		{
			if (pPiggie->GetBody()->IsEnabled())
			{
				glm::vec2 piggiePos = Math::Box2DtoVec2(pPiggie->GetBody()->GetPosition());
				float piggieRadius = pPiggie->GetRadius();
				pPiggie->SetPRS(piggiePos.x, piggiePos.y, glm::degrees(pPiggie->GetBody()->GetAngle()), piggieRadius * 2.0f, piggieRadius * 2.0f);
			}
		}

		MoveNextFireableBoid();
		RemoveDeadPiggies();
		RemoveDamagedBoxes();
		UpdateGameState();

		//Update physics simulation only during play
		m_World->Step(1.0f / 60.0f, 6, 6);
	}

	//Update sounds
	//m_audioSystem->update();

	//Tell glut to call the render function again
	glutPostRedisplay();
}

void GameManager::UpdateGameState()
{
	//Win/lose state
	if ((m_boidsLeft == 0 && m_piggiesAlive > 0 && m_clock.GetTimeElapsedS() - m_timeOfLastFiredBoid > 5.0 && !m_lastSelectedBoid->GetBody()->IsAwake())
		|| (m_piggiesAlive == 0 && m_clock.GetTimeElapsedS() - m_timeOfLastPiggieDeath > 5.0))
	{
		m_gameState = GAME_OVER;
	}
}

void GameManager::Render()
{
	//Clear the screen before every frame
	Clear();

	if (m_gameState == GAME_MENU)
	{
		m_menuTitleText->Render();
		m_menuInstructText->Render();
	}
	else if (m_gameState == GAME_PLAY)
	{
		m_gameScoreText->Render();

		m_physicsSeesaw->Render(*m_camera);

		for (auto& pBox : m_physicsBoxes)
		{
			if (pBox->GetBody()->IsEnabled())
			{
				pBox->Render(*m_camera);
			}
		}

		for (auto& pBoid : m_angryBoids)
		{
			pBoid->Render(*m_camera);
		}

		for (auto& pPiggie : m_piggies)
		{
			if (pPiggie->GetBody()->IsEnabled())
			{
				pPiggie->Render(*m_camera);
			}
		}

		if (m_drawLine)
		{
			glBegin(GL_LINES);
			glVertex2f((float)m_shootLinePoint1, (float)m_shootLinePoint2);
			glVertex2f((float)m_shootLinePoint3, (float)m_shootLinePoint4);
			glEnd();
		}
	}
	else if (m_gameState == GAME_OVER)
	{
		m_gameOverTitleText->Render();
		m_gameOverInstructText->Render();
	}

	glutSwapBuffers();
	u_frameNum++;
}

void GameManager::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void GameManager::Reset()
{
	m_timeOfLastFiredBoid = 0.0f;

	if (m_mouseJoint)
	{
		m_World->DestroyJoint(m_mouseJoint);
		m_mouseJoint = nullptr;
	}

	m_selectedBoid = nullptr;
	m_lastSelectedBoid = nullptr;
	m_nextBoidToFire = nullptr;

	int i = 0;
	for (auto box : m_physicsBoxes)
	{
		auto boxBody = box->GetBody();
		boxBody->SetEnabled(true);
		boxBody->SetTransform(Math::Vec2toBox2D(box->GetOriginalPosition()), 0.0f);
		boxBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		boxBody->SetAngularVelocity(0.0f);
		boxBody->SetAwake(true);
		box->SetDrawnTex(0);
		box->SetTimeOfDeath(0.0);
		box->SetIsDestroyed(false);
	}
	
	i = 0;
	for (auto angryBoid : m_angryBoids)
	{
		//Set it so you can fire the first bird again
		if (i == 0) { angryBoid->SetFireable(true); } else { angryBoid->SetFireable(false); }
			
		angryBoid->GetBody()->SetTransform(Math::Vec2toBox2D(angryBoid->GetOriginalPosition()), 0.0f);
		angryBoid->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		angryBoid->GetBody()->SetAngularVelocity(0.0f);
		angryBoid->GetBody()->SetGravityScale(0.0f);
		angryBoid->GetBody()->SetType(b2_staticBody);

		i++;
	}

	for (auto piggie : m_piggies)
	{
		auto piggieBody = piggie->GetBody();
		piggieBody->SetEnabled(true);
		piggieBody->SetTransform(Math::Vec2toBox2D(piggie->GetOriginalPosition()), 0.0f);
		piggieBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		piggieBody->SetAngularVelocity(0.0f);
		piggieBody->SetAwake(true);
		piggie->SetDrawnTex(0);
		piggie->SetTimeOfDeath(0.0);
		piggie->SetIsDead(false);
	}

	m_physicsSeesaw->GetBody()->SetTransform(Math::Vec2toBox2D(glm::vec2(400.0f, 0.0f)), 0.0f);
	m_physicsSeesaw->GetBody()->SetAngularVelocity(0.0f);

	m_gameScore = 0;
	m_piggiesAlive = 4;
	m_boidsLeft = 4;

	//Update gamescore
	std::string tempText = "Score: ";
	tempText = tempText + std::to_string(m_gameScore);
	m_gameScoreText->SetText(tempText);
}

void GameManager::CheckMouseToBoidCollisions()
{
	//If the left mouse button was clicked this frame and there is no mouse joint
	if (inputManager.MouseState[0] == INPUT_DOWN_FIRST && !m_mouseJoint)
	{
		//If clicked on a boid that can be fire create a mouse joint
		for (auto& angryBoid : m_angryBoids)
		{
			//If the current boid can't be fired skip it
			if (!angryBoid->GetFireable())
			{
				continue;
			}

			//Get the position of the angry boid
			glm::vec2 angryBoidPos = Math::Box2DtoVec2(angryBoid->GetBody()->GetPosition());
			float angryBoidRadius = angryBoid->GetRadius();

			//If the distance is negative, then the click is inside the boid
			bool mouseCollided = float(glm::distance(angryBoidPos, m_leftMouseDownPos) - angryBoidRadius) < 0.0f;

			//Check if the mouse is colliding with the angry boid and we can fire it
			if (mouseCollided)
			{
				//The player clicked on the angry boid
				std::cout << "Colliding with angry boid!" << std::endl;

				//Set the currently selected boid to the angry boid
				m_selectedBoid = angryBoid.get();
				m_lastSelectedBoid = m_selectedBoid;
				b2Body& angryBoidBody = *(angryBoid->GetBody());

				//Set the boid body to dynamic so it can move
				angryBoidBody.SetType(b2_dynamicBody);

				//Create the mouse joint
				b2MouseJointDef md;
				md.bodyA = m_groundBody;
				md.bodyB = &angryBoidBody;

				//The target is where the mousejoint will move the angry boid
				//Target is the mouse position
				md.target = Math::Vec2toBox2D(glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY));
				md.maxForce = 1000.0f * angryBoidBody.GetMass();
				m_mouseJoint = (b2MouseJoint*)m_World->CreateJoint(&md);

				//Wake it up so it applies the force
				angryBoidBody.SetAwake(true);
				break;
			}
		}
	}

	//If the mouse has been moved while the mouse joint exists
	if (m_leftMBDown && m_mouseJoint)
	{
		//Update the target to the new mouse pos
		glm::vec2 vecToMouse = glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY) - m_leftMouseDownPos;
		if (glm::length(vecToMouse) > 400.0f)
		{
			Math::LimitVector2D(vecToMouse, 400.0f);
		}
		b2Vec2 newPos = Math::Vec2toBox2D(m_leftMouseDownPos + vecToMouse);
		m_mouseJoint->SetTarget(newPos);

		//Tried to draw a line to show which direction the bird is going to get shot
		glm::vec2 forceVec = m_leftMouseDownPos - glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY);
		//forceVec *= 2;

		//Calculate the shoot line 
		m_drawLine = true;
		m_shootLinePoint1 = Math::remap(inputManager.g_mousePosX, -inputManager.GL_HSCREEN_WIDTH,  inputManager.GL_HSCREEN_WIDTH,  -1.0, 1.0);
		m_shootLinePoint2 = Math::remap(inputManager.g_mousePosY, -inputManager.GL_HSCREEN_HEIGHT, inputManager.GL_HSCREEN_HEIGHT, -1.0, 1.0);
		auto temp1        = Math::remap(m_leftMouseDownPos.x,     -inputManager.GL_HSCREEN_WIDTH,  inputManager.GL_HSCREEN_WIDTH,  -1.0, 1.0);
		auto temp2        = Math::remap(m_leftMouseDownPos.y,     -inputManager.GL_HSCREEN_HEIGHT, inputManager.GL_HSCREEN_HEIGHT, -1.0, 1.0);
		glm::vec2 tempPos = glm::vec2(temp1, temp2) - glm::vec2(m_shootLinePoint1, m_shootLinePoint2);
		m_shootLinePoint3 = tempPos.x;
		m_shootLinePoint4 = tempPos.y;
	}

	//If the mouse has been released and the mouse joint exists
	if (!m_leftMBDown && m_mouseJoint)
	{
		m_World->DestroyJoint(m_mouseJoint);
		m_mouseJoint = nullptr;

		m_drawLine = false;

		//Make the angry boid get shot towards where it was first clicked on
		b2Body& boidBody = *(m_selectedBoid->GetBody());

		//Calculate the force vector
		glm::vec2 forceVec = m_leftMouseDownPos - m_leftMouseUpPos;

		//Only do something when the mouse was moved
		if (glm::length(forceVec) != 0)
		{
			forceVec *= boidBody.GetMass() * 100.0f;

			//Apply the force to the angry boid so that it gets flung when the mouse is let go
			boidBody.ApplyForce(Math::Vec2toBox2D(forceVec), boidBody.GetPosition(), true);
			boidBody.SetGravityScale(1.0f);


			//Decrement the bird counter
			m_boidsLeft--;

			m_selectedBoid->SetFireable(false);

			//Find the position of the selected boid in the vector (this will always find something)
			auto iteratorToSelectedBoid = std::find_if(m_angryBoids.begin(), m_angryBoids.end(),
				[this](std::shared_ptr<AngryBoid> const& i) { return i.get() == m_selectedBoid; });

			//If incrementing the iterator reaches the end, then the player has fired all the boids
			if (++iteratorToSelectedBoid == m_angryBoids.end())
			{
				m_nextBoidToFire = nullptr;
			}
			//If incrementing the iterator does not reach the end, then there is another boid that can be fired
			else
			{
				m_nextBoidToFire = iteratorToSelectedBoid->get();
				m_timeOfLastFiredBoid = m_clock.GetTimeElapsedS();
			}
		}
		m_selectedBoid = nullptr;
	}

	//Test physics - uncommenting this will allow you to have the boxes fly towards the mouse
	/*if (m_leftMBDown)
	{
		for (auto& pBox : m_physicsBoxes)
		{
			glm::vec2 mousePos = glm::vec2(inputManager.g_mousePosX, inputManager.g_mousePosY);
			auto boxPos = Math::Box2DtoVec2(pBox->GetBody()->GetPosition());
			glm::vec2 vecToMouseClick = mousePos - boxPos;

			vecToMouseClick = glm::normalize(vecToMouseClick) * 1000.0f;

			pBox->GetBody()->ApplyForce(Math::Vec2toBox2D(vecToMouseClick), pBox->GetBody()->GetPosition(), true);
		}
	}*/
}

void GameManager::MoveNextFireableBoid()
{
	//If there is a boid to fire and it has been over 1.5 seconds since the previous boid was fired
	//Then move the next boid into the fire position
	if (m_nextBoidToFire != nullptr && m_clock.GetTimeElapsedS() - m_timeOfLastFiredBoid > 1.5)
	{
		m_nextBoidToFire->GetBody()->SetTransform(Math::Vec2toBox2D(m_boidFirePos), 0.0f);
		m_nextBoidToFire->SetFireable(true);
		m_nextBoidToFire = nullptr;
	}
}

void GameManager::RemoveDeadPiggies()
{
	for (auto& piggie : m_piggies)
	{
		if (piggie->GetIsDead())
		{
			//If the piggie has only just died set the time of death
			if (!(piggie->GetTimeOfDeath() > 0.0))
			{
				//Update gamescore
				m_gameScore++;
				std::string tempText = "Score: ";
				tempText = tempText + std::to_string(m_gameScore);
				m_gameScoreText->SetText(tempText);

				m_piggiesAlive--;
				m_timeOfLastPiggieDeath = m_clock.GetTimeElapsedS();
				piggie->SetTimeOfDeath(m_clock.GetTimeElapsedS());
			}
			else
			{
				//If piggie has been dead for 2 seconds now
				if (piggie->GetBody()->IsEnabled() && m_clock.GetTimeElapsedS() - piggie->GetTimeOfDeath() > 2.0)
				{
					//Disable the piggies body
					auto tempRef = piggie->GetBody();
					//tempRef->SetTransform(Math::Vec2toBox2D(glm::vec2(1000.0f, 1000.0f)), 0.0f);
					tempRef->SetEnabled(false);
				}
			}
		}
	}
}

void GameManager::RemoveDamagedBoxes()
{
	for (auto& box : m_physicsBoxes)
	{
		if (box->GetDestructable() && box->GetIsDestroyed())
		{
			//If the piggie has only just died set the time of death
			if (!(box->GetTimeOfDeath() > 0.0))
			{
				box->SetTimeOfDeath(m_clock.GetTimeElapsedS());
			}
			else
			{
				//If piggie has been dead for 2 seconds now
				if (box->GetBody()->IsEnabled() && m_clock.GetTimeElapsedS() - box->GetTimeOfDeath() > 2.0)
				{
					//Disable the destroyed box
					auto tempRef = box->GetBody();

					tempRef->SetEnabled(false);
				}
			}
		}
	}
}

void GameManager::CreateScreenBorderWalls()
{
	//Ground border
	b2Vec2 tempPos = Math::Vec2toBox2D(glm::vec2(0.0f, -inputManager.GL_SCREEN_HEIGHT));
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(tempPos.x, tempPos.y);
	b2Body* groundBody = m_World->CreateBody(&groundBodyDef);

	//Make the ground fixture
	auto tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.GL_HSCREEN_WIDTH, inputManager.GL_HSCREEN_HEIGHT));
	b2PolygonShape groundBox;
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Left wall border
	tempPos = Math::Vec2toBox2D(glm::vec2(-inputManager.GL_SCREEN_WIDTH, 00.0f));
	b2BodyDef leftWallBody;
	leftWallBody.position.Set(tempPos.x, tempPos.y);
	groundBody = m_World->CreateBody(&leftWallBody);

	// Make the fixture
	tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.GL_HSCREEN_WIDTH, inputManager.GL_HSCREEN_HEIGHT));
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Right wall border
	tempPos = Math::Vec2toBox2D(glm::vec2(inputManager.GL_SCREEN_WIDTH, 0.0f));
	b2BodyDef rightWallBody;
	rightWallBody.position.Set(tempPos.x, tempPos.y);
	groundBody = m_World->CreateBody(&rightWallBody);

	// Make the fixture
	tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.GL_HSCREEN_WIDTH, inputManager.GL_HSCREEN_HEIGHT));
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Ground border
	tempPos = Math::Vec2toBox2D(glm::vec2(0.0f, inputManager.GL_SCREEN_HEIGHT));
	b2BodyDef roofBodyDef;
	roofBodyDef.position.Set(tempPos.x, tempPos.y);
	groundBody = m_World->CreateBody(&roofBodyDef);

	// Make the ground fixture
	tempSize = Math::Vec2toBox2D(glm::vec2(inputManager.GL_HSCREEN_WIDTH, inputManager.GL_HSCREEN_HEIGHT));
	groundBox.SetAsBox(tempSize.x, tempSize.y);
	groundBody->CreateFixture(&groundBox, 0.0f);
}
