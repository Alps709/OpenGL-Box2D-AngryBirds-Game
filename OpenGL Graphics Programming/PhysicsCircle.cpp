#include "PhysicsCircle.h"
#include "Obj.h"

PhysicsCircle::PhysicsCircle(b2World* world, const glm::vec2& position, float _radius, float density)
{
	m_radius = _radius;
	m_world = world;

	// Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 0.0f;
	const auto tempPos = Math::Vec2toBox2D(position);
	bodyDef.position.Set(tempPos.x, tempPos.y);
	m_body = world->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0); //position, relative to body position
	circleShape.m_radius = Math::FloatToBox2D(_radius);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);

	m_mesh = Mesh(Objects::verticesBox, Objects::indicesBox);
	m_shader = Shader();
}

PhysicsCircle::~PhysicsCircle()
{
	//m_world->DestroyBody(m_body);
}