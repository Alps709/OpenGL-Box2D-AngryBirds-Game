#pragma once
#include "PhysicsCircle.h"

class AngryBoid : public PhysicsCircle
{
public:
	AngryBoid() = default;
	AngryBoid(b2World * _world, const glm::vec2 & _position, float _radius, float _density = 1.0f);
	~AngryBoid() = default;

	void SetFireable(bool _fireable) { m_fireable = _fireable; }

	bool GetFireable() { return m_fireable; }
	glm::vec2 GetOriginalPosition() { return m_originalPosition; }

private:

	bool m_fireable;

	bool m_beenFired;

	glm::vec2 m_originalPosition;

	std::string m_tag;
};

