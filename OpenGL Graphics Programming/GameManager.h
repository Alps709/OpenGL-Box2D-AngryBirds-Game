#pragma once
#include "InputManager.h"
#include "TextLabel.h"
#include "Texture.h"
#include "Object.h"
#include "clock.h"
#include "PhysicsBox.h"
#include "PhysicsCircle.h"
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

	//Create the physical screen borders for the box 2d world
	void CreateScreenBorders();

	void Update(int _mousePosX, int _mousePosY);
	void Render();
	void Clear();

	TextLabel* m_boidStateText = nullptr;
	TextLabel* m_containmentStateText = nullptr;

	inline static GameState m_gameState = GAME_MENU;
	inline static GameplayState m_gameplayState = PLAY_SEEK;

private:
	InputManager& inputManager = InputManager::getInstance();

	//Game variables
	unsigned int m_gameScore = 0;
	
	//Clock
	CClock m_clock;
	double m_pillSpawnTimer = 0.0;

	//Camera
	Camera* m_camera = nullptr;

	//Box 2D stuff
	std::unique_ptr<b2World> m_World;
	b2Vec2 m_gravity = { 0.0, -1.0 };

	///Objects

	//Box 2D Physics Objects
	std::vector<PhysicsBox> m_physicsBoxes;
	std::vector<PhysicsCircle> m_physicsCircles;

	//Game Background
	Object m_backgroundObject;
	Mesh* m_backgroundMesh = nullptr;
	Texture* m_backgroundTexture = nullptr;

	//Circle Texture
	Texture* m_circleTexture = nullptr;

	//Default shader
	Shader* m_defaultShader = nullptr;

	//Text
	TextLabel* m_menuTitleText = nullptr;
	TextLabel* m_menuInstructText = nullptr;
	
	//Audio
	inline static FMOD::System* m_audioSystem = nullptr;
	inline static FMOD::Sound* m_yeatSound = nullptr;
	inline static FMOD::Sound* m_shoopSound = nullptr;
	inline static FMOD::Sound* m_trackBackground = nullptr;

	//Uniforms
	GLfloat u_currentTime = 0;
	int u_frameNum = 0;

	//Debug stuff
	bool WireframeRenderMode = false;
};

