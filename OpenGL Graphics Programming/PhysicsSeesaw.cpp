#include "PhysicsSeesaw.h"
#include "Obj.h"

PhysicsSeesaw::PhysicsSeesaw(b2World* world, const glm::vec2& position, const glm::vec2& size, float density)
{
	m_size = size;
	m_world = world;

	// Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	const auto tempPos = Math::Vec2toBox2D(position);
	bodyDef.position.Set(tempPos.x, tempPos.y);
	m_body = m_world->CreateBody(&bodyDef);

	const auto tempShape = Math::Vec2toBox2D(size);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(tempShape.x / 2.0f, tempShape.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);

	b2BodyDef groundbodyDef;
	groundbodyDef.position.Set(tempPos.x, tempPos.y);
	m_groundBody = world->CreateBody(&groundbodyDef);

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = m_groundBody;
	revoluteJointDef.bodyB = m_body;
	revoluteJointDef.collideConnected = false;

	m_seesawJoint = (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);

	m_mesh = Mesh(Objects::verticesBox, Objects::indicesBox);
	m_shader = Shader();

	m_tag = "Seesaw";
	m_userData = std::make_shared<PhysicsObjectData>(m_tag, m_tex0, m_tex1, &m_drawnTex, nullptr, nullptr);
	m_body->SetUserData(static_cast<void*>(m_userData.get()));
}

PhysicsSeesaw::~PhysicsSeesaw()
{
	m_world->DestroyJoint(m_seesawJoint);
	m_world->DestroyBody(m_groundBody);
	m_world->DestroyBody(m_body);
}