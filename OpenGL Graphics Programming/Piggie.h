#pragma once
#include "PhysicsCircle.h"

class Piggie : public PhysicsCircle
{
public:
	Piggie() = default;
	Piggie(b2World * _world, const glm::vec2 & _position, float _radius, float _density = 1.0f);
	~Piggie();

	void SetUserData();
	void SetTimeOfDeath(double _time) { m_timeOfDeath = _time; }
	void SetIsDead(bool _isDead) { *m_dead = _isDead; }

	bool GetIsDead() { return *m_dead; }

	//In seconds
	double GetTimeOfDeath() { return m_timeOfDeath; }

	glm::vec2 GetOriginalPosition() { return m_originalPosition; }

private:

	double m_timeOfDeath = 0.0;

	bool* m_dead;

	bool m_toBeDestroyed;

	glm::vec2 m_originalPosition;

	std::shared_ptr<PhysicsObjectData> m_userData;
};

