#include "PhysicsBox.h"
#include "Obj.h"
#include "Math.h"

PhysicsBox::PhysicsBox(b2World* _world, const glm::vec2& _position, const glm::vec2& _size, float _density)
{
	m_size = _size;
	m_originalPosition = _position;
	m_world = _world;

	// Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	const auto tempPos = Math::Vec2toBox2D(_position);
	bodyDef.position.Set(tempPos.x, tempPos.y);
	m_body = m_world->CreateBody(&bodyDef);

	const auto tempShape = Math::Vec2toBox2D(_size);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(tempShape.x / 2.0f, tempShape.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = _density;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);

	m_mesh = Mesh(Objects::verticesBox, Objects::indicesBox);
	m_shader = Shader();
}

PhysicsBox::~PhysicsBox()
{
	delete m_hasDied;
	m_world->DestroyBody(m_body);
}

void PhysicsBox::SetUserData()
{
	m_userData = std::make_shared<PhysicsObjectData>(m_tag, m_tex0, m_tex1, &m_drawnTex, m_hasDied, m_destructable);
	m_body->SetUserData(static_cast<void*>(m_userData.get()));
}
