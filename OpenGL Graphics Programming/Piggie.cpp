#include "Piggie.h"

Piggie::Piggie(b2World* _world, const glm::vec2& _position, float _radius, float _density)
	: PhysicsCircle(_world, _position, _radius, _density), 
	m_dead(new bool(false)), 
	m_toBeDestroyed(false), 
	m_originalPosition(_position)
{
	m_body->SetType(b2_dynamicBody);
	m_body->SetGravityScale(1.0f);
}

Piggie::~Piggie()
{
	delete m_dead;
}

void Piggie::SetUserData()
{
	m_tag = "Piggie";
	m_userData = std::make_shared<PhysicsObjectData>("Piggie", m_tex0, m_tex1, &m_drawnTex, m_dead, nullptr);
	m_body->SetUserData(static_cast<void*>(m_userData.get()));
}
