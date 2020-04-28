#pragma once

class InputManager
{

public:
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}
	
	void Update();

	enum InputState
	{
		INPUT_NULL = 0,
		INPUT_UP = 1,
		INPUT_DOWN = 2,
		INPUT_UP_FIRST = 3,
		INPUT_DOWN_FIRST = 4,
	};

	int g_mousePosX = 0;
	int g_mousePosY = 0;
	int g_mousePosDifX = 0;
	int g_mousePosDifY = 0;

	//Game window size
	float GL_SCREEN_WIDTH = 1280.0f;
	float GL_SCREEN_HEIGHT = 720.0f;
	float GL_HSCREEN_WIDTH = GL_SCREEN_WIDTH / 2.0f;
	float GL_HSCREEN_HEIGHT = GL_SCREEN_HEIGHT / 2.0f;

	//True window size (set in the windowResize function callback)
	float WINDOW_SCREEN_WIDTH = GL_SCREEN_WIDTH;
	float WINDOW_SCREEN_HEIGHT = GL_SCREEN_HEIGHT;
	float WINDOW_HSCREEN_WIDTH = GL_HSCREEN_WIDTH;
	float WINDOW_HSCREEN_HEIGHT = GL_HSCREEN_HEIGHT;

	//Mouse Input
	InputState MouseState[3];

	//Keyboard input
	InputState KeyState[255];
	InputState SpecialKeyState[255];

private:
	InputManager() {}                  

public:
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
};

