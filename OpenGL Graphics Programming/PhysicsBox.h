#pragma once

#include <box2d.h>
#include "Object.h"


class PhysicsBox: public Object
{
public:
	PhysicsBox() = default;
	PhysicsBox(b2World* world, const glm::vec2& position, const glm::vec2& size, float density = 1.0f);
	~PhysicsBox() override;

	void SetUserData();
	void SetDestructable(bool _destructable) { m_destructable = _destructable; }
	void SetIsDestroyed(bool _destroyed) { *m_hasDied = _destroyed; }
	void SetTimeOfDeath(double _timeOfDeath) { m_timeOfDeath = _timeOfDeath; }

	//Getters
	glm::vec2 GetOriginalPosition() { return m_originalPosition; }
	bool GetDestructable() { return m_destructable; }
	bool GetIsDestroyed() { return *m_hasDied; }
	double GetTimeOfDeath() { return m_timeOfDeath; }
	b2Body* GetBody(){ return m_body; }
	glm::vec2 GetSize(){ return m_size; }

protected:
	b2World* m_world = nullptr;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_position{ 0.0f, 0.0f };
	glm::vec2 m_originalPosition;
	float m_rotationZ = 0.0f;
	glm::vec2 m_size{ 1.0f, 1.0f };
	double m_colliderRadius = 10.0f;

	std::shared_ptr<PhysicsObjectData> m_userData;
	std::string m_tag = "Box";
	
	bool m_destructable = false;
	bool* m_hasDied = new bool(false);
	double m_timeOfDeath = 0.0;
	
};


