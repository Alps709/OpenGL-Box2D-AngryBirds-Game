#pragma once
#include "InputManager.h"
#include "TextLabel.h"
#include "Texture.h"
#include "Object.h"
#include "clock.h"
#include "PhysicsBox.h"
#include "PhysicsCircle.h"
#include "PhysicsSeesaw.h"
#include <box2d.h>

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

	void CheckMouseCollisions();

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
	double m_pillSpawnTimer = 0.0;

	//Camera
	std::unique_ptr<Camera> m_camera;

	//Box 2D stuff
	std::unique_ptr<b2World> m_World;
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
	std::vector< std::shared_ptr<PhysicsCircle>> m_physicsCircles;
	PhysicsCircle* m_selectedBoid = nullptr;

	//Game Background
	Object m_backgroundObject;
	Mesh m_backgroundMesh;
	Texture* m_backgroundTexture;

	//Circle Texture
	Texture* m_angryBoidTexture;

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

