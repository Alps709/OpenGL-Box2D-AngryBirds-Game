#pragma once
#include "Debug.h"
#include "InputManager.h"
#include "Math.h"
#include <freeglut.h>

//Callback functions for glut input
namespace
{
	inline InputManager& inputManagerI = InputManager::getInstance();

	enum InputState
	{
		INPUT_NULL = 0, //Default key state (can be used to tell is a key has ever been pressed)
		INPUT_UP = 1,
		INPUT_DOWN = 2,
		INPUT_UP_FIRST = 3,
		INPUT_DOWN_FIRST = 4,
	};

	//Declared inputs to be handled that have an uppercase and lowercase version
	unsigned char inputs[10] = { 'w', 'a', 's', 'd', 'p', 'W', 'A', 'S', 'D', 'P' };

	void MouseClick(int _button, int _state, int _x, int _y)
	{
		//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
		_x = (int)(Math::remap(_x, -inputManagerI.TRUE_HSCREEN_WIDTH, inputManagerI.TRUE_HSCREEN_WIDTH, -inputManagerI.HSCREEN_WIDTH, inputManagerI.HSCREEN_WIDTH) - inputManagerI.HSCREEN_WIDTH);
		_y = (int)(Math::remap(_y, -inputManagerI.TRUE_HSCREEN_HEIGHT, inputManagerI.TRUE_HSCREEN_HEIGHT, -inputManagerI.HSCREEN_HEIGHT, inputManagerI.HSCREEN_HEIGHT) - inputManagerI.HSCREEN_HEIGHT);

		//Invert y axis
		_y *= -1;

		//We are not using any buttons higher than 2, so don't bother doing anything
		if (_button > 2)
		{
			return;
		}

		//Get the x and y difference since last frame
		inputManagerI.g_mousePosDifX = _x - inputManagerI.g_mousePosX;
		inputManagerI.g_mousePosDifY = _y - inputManagerI.g_mousePosY;

		//Set mouse pos
		inputManagerI.g_mousePosX = _x;
		inputManagerI.g_mousePosY = _y;

		inputManagerI.MouseState[_button] = (GLUT_DOWN == _state) ? inputManagerI.INPUT_DOWN_FIRST : inputManagerI.INPUT_UP_FIRST;
		//Debug logging
		//std::cout << "Mouse clicked on - x: " << _x << " | y: " << _y << std::endl;
	}

	void MousePassiveMove(int _x, int _y)
	{
		//Convert current screen width and height mouse co-ords to 
		//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
		//(remaps screen size mouse coords to opengl pixel coords)
		_x = (int)(Math::remap(_x, -inputManagerI.TRUE_HSCREEN_WIDTH, inputManagerI.TRUE_HSCREEN_WIDTH, -inputManagerI.HSCREEN_WIDTH, inputManagerI.HSCREEN_WIDTH) - inputManagerI.HSCREEN_WIDTH);
		_y = (int)(Math::remap(_y, -inputManagerI.TRUE_HSCREEN_HEIGHT, inputManagerI.TRUE_HSCREEN_HEIGHT, -inputManagerI.HSCREEN_HEIGHT, inputManagerI.HSCREEN_HEIGHT) - inputManagerI.HSCREEN_HEIGHT);

		//Invert y axis
		_y *= -1;

		inputManagerI.g_mousePosX = _x;
		inputManagerI.g_mousePosY = _y;

		////Debug logging
		//std::cout << "Mouse moved to - x: " << _x << " | y: " << _y << std::endl;
		//std::cout << "Mouse change in - x: " << inputManager.g_mousePosDifX << " | y: " << inputManager.g_mousePosDifY << std::endl;
	}

	void MouseMove(int _x, int _y)
	{
		//Convert current screen width and height mouse co-ords to 
		//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
		//(remaps screen size mouse coords to opengl pixel coords)
		_x = (int)(Math::remap(_x, -inputManagerI.TRUE_HSCREEN_WIDTH, inputManagerI.TRUE_HSCREEN_WIDTH, -inputManagerI.HSCREEN_WIDTH, inputManagerI.HSCREEN_WIDTH) - inputManagerI.HSCREEN_WIDTH);
		_y = (int)(Math::remap(_y, -inputManagerI.TRUE_HSCREEN_HEIGHT, inputManagerI.TRUE_HSCREEN_HEIGHT, -inputManagerI.HSCREEN_HEIGHT, inputManagerI.HSCREEN_HEIGHT) - inputManagerI.HSCREEN_HEIGHT);

		//Invert y axis
		_y *= -1;

		inputManagerI.g_mousePosX = _x;
		inputManagerI.g_mousePosY = _y;

		//Debug logging
		//std::cout << "Mouse clicked on - x: " << _x << " | y: " << _y << std::endl;
		//std::cout << "Mouse change in - x: " << g_mousePosDifX << " | y: " << g_mousePosDifY << std::endl;
	}

	void KeyBoardUp(unsigned char _key, int _x, int _y)
	{
		const int tempState = inputManagerI.KeyState[_key];

		//Find offset of the corresponding uppercase or lowercase key
		const int offset = (_key < 97) ? 32 : -32;

		//If the key is down
		if (tempState != inputManagerI.INPUT_UP && tempState != inputManagerI.INPUT_UP_FIRST)
		{
			//Loop through the currently handled input keys
			for (unsigned char input : inputs)
			{
				//If the key is declared to be handled
				if (_key == input)
				{
					//Set the the corresponding uppercase or lowercase key aswell
					inputManagerI.KeyState[_key + offset] = inputManagerI.INPUT_UP_FIRST;
					break;
				}
			}
			//Continue to change keystate as normal
			inputManagerI.KeyState[_key] = inputManagerI.INPUT_UP_FIRST;
		}
	}

	void KeyBoardDown(unsigned char _key, int _x, int _y)
	{
		const int tempState = inputManagerI.KeyState[_key];
		const int offset = (_key < 97) ? 32 : -32;

		//Key is up
		if (tempState != inputManagerI.INPUT_DOWN && tempState != inputManagerI.INPUT_DOWN_FIRST)
		{
			for (unsigned char input : inputs)
			{
				if (_key == input)
				{
					inputManagerI.KeyState[_key + offset] = inputManagerI.INPUT_DOWN_FIRST;
					break;
				}
			}
			inputManagerI.KeyState[_key] = inputManagerI.INPUT_DOWN_FIRST;
		}
	}

	//Special key up function
	void SpecialKeyBoardUp(int _key, int _x, int _y)
	{
		const int tempState = inputManagerI.SpecialKeyState[_key];

		if (tempState != inputManagerI.INPUT_UP && tempState != inputManagerI.INPUT_UP_FIRST)
		{
			inputManagerI.SpecialKeyState[_key] = inputManagerI.INPUT_UP_FIRST;
		}
	}

	//Special key down function
	void SpecialKeyBoardDown(int _key, int _x, int _y)
	{
		const int tempState = inputManagerI.SpecialKeyState[_key];

		if (tempState != inputManagerI.INPUT_DOWN && tempState != inputManagerI.INPUT_DOWN_FIRST)
		{
			inputManagerI.SpecialKeyState[_key] = inputManagerI.INPUT_DOWN_FIRST;
		}
	}

	void WindowResize(int _x, int _y)
	{
		inputManagerI.TRUE_SCREEN_WIDTH = (float)_x;
		inputManagerI.TRUE_SCREEN_HEIGHT = (float)_y;
		inputManagerI.TRUE_HSCREEN_HEIGHT = inputManagerI.TRUE_SCREEN_HEIGHT / 2;
		inputManagerI.TRUE_HSCREEN_WIDTH = inputManagerI.TRUE_SCREEN_WIDTH / 2;

		glViewport((GLsizei)0, (GLsizei)0, (GLsizei)inputManagerI.TRUE_SCREEN_WIDTH, (GLsizei)inputManagerI.TRUE_SCREEN_HEIGHT);
	}
}
