#include "PhysicsBox.h"
#include "Obj.h"

PhysicsBox::PhysicsBox(b2World* world, const glm::vec2& position, const glm::vec2& size, float density)
{
	m_size = size;

	// Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	const auto tempPos = Math::Vec2toBox2D(position);
	bodyDef.position.Set(tempPos.x, tempPos.y);
	m_body = world->CreateBody(&bodyDef);

	const auto tempShape = Math::Vec2toBox2D(size);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(tempShape.x / 2.0f, tempShape.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);

	m_mesh = Mesh(Objects::verticesBox, Objects::indicesBox);
	m_shader = Shader();
}

PhysicsBox::~PhysicsBox()
{
}