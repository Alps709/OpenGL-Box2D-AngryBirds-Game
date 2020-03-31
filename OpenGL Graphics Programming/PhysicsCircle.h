#pragma once

#include <box2d.h>
#include "Object.h"
#include "Math.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <detail/type_vec3.hpp>
#include <detail/type_mat4x4.hpp>

class PhysicsCircle : public Object
{
public:
	PhysicsCircle() = default;
	PhysicsCircle(b2World* world, const glm::vec2& position, float _radius, float density = 1.0f);
	~PhysicsCircle();

	//Getters
	b2Body* GetBody() { return m_body; }
	float GetRadius() { return m_radius; }

protected:

	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_position{ 0.0f, 0.0f };
	float m_rotationZ = 0.0f;
	float m_radius = 1.0f;
	double m_colliderRadius = 10.0f;
};


