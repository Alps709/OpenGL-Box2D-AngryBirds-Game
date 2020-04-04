#pragma once
#include <vector>
#include "InputManager.h"

namespace Objects
{
	inline InputManager& inputManagerO = InputManager::getInstance();

	static std::vector<float> verticesBoid =
	{
		///Bottom right quad
		//Position			    //Colour			      //Texture co-ords
		  0.0f,  20.0f, 0.0f,   255.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Middle top
		-10.0f, -10.0f, 0.0f,   255.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Bottom left
		 10.0f, -10.0f, 0.0f,   255.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Bottom right
	};

	static std::vector<unsigned int> indicesBoid =
	{
		//triangle							
		0, 2, 1,
	};

	static std::vector<float> verticesBackground =
	{
		///Bottom right quad
		//Position			     //Colour			        //Texture co-ords
		-inputManagerO.HSCREEN_WIDTH, -inputManagerO.HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    //Bottom left
		-inputManagerO.HSCREEN_WIDTH,  inputManagerO.HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    //Top left
		 inputManagerO.HSCREEN_WIDTH,  inputManagerO.HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    //Top right
		 inputManagerO.HSCREEN_WIDTH, -inputManagerO.HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    //Bottom right
	};

	static std::vector<unsigned int> indicesBackground =
	{
		//First quad							
		0, 2, 1,
		0, 3, 2,
	};

	static std::vector<float> verticesBox =
	{
		///Bottom right quad
		//Position			     //Colour			        //Texture co-ords
		-0.5f, -0.5f, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    //Bottom left
		-0.5f,  0.5f, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    //Top left
		 0.5f,  0.5f, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    //Top right
		 0.5f, -0.5f, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    //Bottom right
	};

	static std::vector<unsigned int> indicesBox =
	{
		//First quad							
		0, 2, 1,
		0, 3, 2,
	};
}
