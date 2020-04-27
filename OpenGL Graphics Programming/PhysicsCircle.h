#pragma once

#include <box2d.h>
#include "Object.h"


class PhysicsCircle : public Object
{
public:
	PhysicsCircle() = default;
	PhysicsCircle(b2World* world, const glm::vec2& position, float _radius, float density = 1.0f);
	~PhysicsCircle() override;

	//Getters
	b2Body* GetBody() { return m_body; }
	float GetRadius() { return m_radius; }
	std::string GetTag() { return m_tag; }

protected:
	b2World* m_world = nullptr;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_position{ 0.0f, 0.0f };
	float m_rotationZ = 0.0f;
	float m_radius = 1.0f;

	std::string m_tag = "Circle";
};


