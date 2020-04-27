#pragma once
#include "PhysicsCircle.h"

class Piggie : public PhysicsCircle
{
public:
	Piggie() = default;
	Piggie(b2World * _world, const glm::vec2 & _position, float _radius, float _density = 1.0f);
	~Piggie() = default;

	void SetFireable(bool _fireable) { m_fireable = _fireable; }

	bool GetFireable() { return m_fireable; }
	glm::vec2 GetOriginalPosition() { return m_originalPosition; }

private:

	bool m_fireable;

	bool m_beenFired;

	bool m_toBeDestroyed;

	glm::vec2 m_originalPosition;
};

