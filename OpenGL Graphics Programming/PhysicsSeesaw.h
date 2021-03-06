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

class PhysicsSeesaw : public Object
{
public:
	PhysicsSeesaw() = default;
	PhysicsSeesaw(b2World* world, const glm::vec2& position, const glm::vec2& size, float density = 1.0f);
	~PhysicsSeesaw() override;

	//Getters
	b2Body* GetBody() { return m_body; }
	glm::vec2 GetSize() { return m_size; }

protected:
	b2World* m_world = nullptr;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;

	//Base ground body used to attach the mouse joint to the world
	b2Body* m_groundBody = nullptr;

	//Seesaw joint
	b2RevoluteJoint* m_seesawJoint = nullptr;

	glm::vec2 m_position{ 0.0f, 0.0f };
	float m_rotationZ = 0.0f;
	glm::vec2 m_size{ 1.0f, 1.0f };
	double m_colliderRadius = 10.0f;

	std::string m_tag;

	std::shared_ptr<PhysicsObjectData> m_userData;
};


