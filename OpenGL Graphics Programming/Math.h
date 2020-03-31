#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <box2d.h>

namespace Math
{
	static glm::mat4 Create2DModelMatrix(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY)
	{
		//Translation matrix
		const glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(_translateX, _translateY, 0.0f));

		//Rotation matrix
		const glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(), glm::radians(_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		//Scale matrix
		const glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(_scaleX, _scaleY, 1.0f));

		//Return model matrix
		return translationMatrix * rotationMatrixZ * scaleMatrix;
	}

	const unsigned int Box2DScale = 100;

	inline glm::vec2 Box2DtoVec2(b2Vec2 _vec)
	{
		return glm::vec2(_vec.x * Box2DScale, _vec.y * Box2DScale);
	}

	inline b2Vec2 Vec2toBox2D(glm::vec2 _vec)
	{
		return b2Vec2(_vec.x / Box2DScale, _vec.y / Box2DScale);
	}

	inline float FloatToBox2D(float _float)
	{
		return _float/Box2DScale;
	}

	inline float Box2DToFloat(float _float)
	{
		return _float * Box2DScale;
	}

	inline void LimitVector2D(glm::vec2& _vec, float _maxVal)
	{
		if(glm::length(_vec) > _maxVal)
		{
			_vec = glm::normalize(_vec) * _maxVal;
		}
	}
}
