#include "Piggie.h"

Piggie::Piggie(b2World* _world, const glm::vec2& _position, float _radius, float _density)
	: PhysicsCircle(_world, _position, _radius, _density), 
	m_fireable(false), 
	m_beenFired(false), 
	m_toBeDestroyed(false), 
	m_originalPosition(_position)
{
	m_tag = "Piggie";
	m_body->SetUserData(this);

	m_body->SetType(b2_dynamicBody);
	m_body->SetGravityScale(1.0f);
}
