#pragma once
#include "InputManager.h"
#include "TextLabel.h"
#include "Texture.h"
#include "Object.h"
#include "clock.h"
#include "PhysicsBox.h"
#include "AngryBoid.h"
#include "Piggie.h"
#include "PhysicsSeesaw.h"
#include "Box2DCollisionListener.h"

#include <fmod.hpp>


enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1,
	GAME_OVER = 2,
};

enum GameplayState
{
	PLAY_SEEK = 0,
	PLAY_ARRIVE = 1,
	PLAY_FOLLOWPATH = 2,
	PLAY_FLOCK = 3
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	//void AudioInitialise();
	void ProcessInput();

	//Handle what happens when the player clicks on a boid
	void CheckMouseToBoidCollisions();

	void MoveNextFireableBoid();

	//Create the physical screen borders for the box 2d world
	void CreateScreenBorderWalls();

	void Update(int _mousePosX, int _mousePosY);
	void Render();
	void Clear();

	void Reset();

	GameState m_gameState = GAME_MENU;

private:
	InputManager& inputManager = InputManager::getInstance();

	bool m_leftMBDown = false;
	glm::vec2 m_leftMouseDownPos{ 0.0 };
	glm::vec2 m_leftMouseUpPos{ 0.0 };


	//Game variables
	unsigned int m_gameScore = 0;
	
	//Clock
	CClock m_clock;

	//Camera
	std::unique_ptr<Camera> m_camera;

	//Box 2D stuff
	std::unique_ptr<b2World> m_World;
	Box2DCollisionListener m_collisionListener;
	b2Vec2 m_gravity = { 0.0, -1.0 };

	//Used for moving the angry boid when clicked on
	b2MouseJoint* m_mouseJoint = nullptr;

	//Base ground body used to attach the mouse joint to the world
	b2Body* m_groundBody = nullptr;
	
	//Seesaw joint
	b2RevoluteJoint* m_seesawJoint = nullptr;


	///Objects

	//Box 2D Physics Objects
	std::shared_ptr <PhysicsSeesaw> m_physicsSeesaw;
	std::vector<std::shared_ptr<PhysicsBox>> m_physicsBoxes;
	std::vector< std::shared_ptr<AngryBoid>> m_angryBoids;
	std::vector< std::shared_ptr<Piggie>> m_piggies;

	//Used for dragging and shooting the boids
	AngryBoid* m_selectedBoid = nullptr;
	AngryBoid* m_nextBoidToFire = nullptr;

	//Used to wait for a certain amount before moving the next boid to the firing position
	double m_timeOfLastFiredBoid = 0.0;

	//The position that a boid will move to when it can be fired
	glm::vec2 m_boidFirePos = glm::vec2(-400.0f, -100.0f);

	//Line vertices for the bird shoot direction line
	bool m_drawLine = false;
	double m_shootLinePoint1;
	double m_shootLinePoint2;
	double m_shootLinePoint3;
	double m_shootLinePoint4;


	//Game Background
	Object m_backgroundObject;
	Mesh m_backgroundMesh;


	//Textures
	Texture* m_backgroundTexture;
	Texture* m_angryBoidTexture;
	Texture* m_piggieTexture;
	Texture* m_piggieTexture1;

	//Default shader
	Shader m_defaultShader;

	//Text
	TextLabel* m_menuTitleText = nullptr;
	TextLabel* m_menuInstructText = nullptr;
	
	//Audio
	//inline static FMOD::System* m_audioSystem = nullptr;
	//inline static FMOD::Sound* m_yeatSound = nullptr;
	//inline static FMOD::Sound* m_shoopSound = nullptr;
	//inline static FMOD::Sound* m_trackBackground = nullptr;

	//Uniforms
	GLfloat u_currentTime = 0;
	int u_frameNum = 0;

	//Debug stuff
	bool WireframeRenderMode = false;
};

