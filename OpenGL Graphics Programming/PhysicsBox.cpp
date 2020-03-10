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

	m_mesh = new Mesh(Objects::verticesBox, Objects::indicesBox);
	m_shader = new Shader();
}

PhysicsBox::~PhysicsBox()
{
}

void PhysicsBox::SetTexture0(Texture* _tex)
{
	m_tex0 = _tex;
}

void PhysicsBox::SetPRS(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY)
{
	m_position = glm::vec2(_translateX, _translateY);

	//Add new rotation and cap it to 360
	m_rotationZ = fmod(_rotationAngle, 360.0f);

	m_size = glm::vec2(_scaleX, _scaleY);

	//Update the stored model matrix
	UpdateModelMat();
}

void PhysicsBox::UpdateModelMat()
{
	//Update the stored model matrix
	m_modelMat = Math::Create2DModelMatrix(m_position.x, m_position.y, m_rotationZ, m_size.x, m_size.y);
}

void PhysicsBox::Render(Camera& _myCamera)
{
	//Prepare renderer (eg. clear buffer, create PVM matrix etc.)
	glm::mat4 projViewMat = _myCamera.GetProjView();

	//Bind the mesh that all the models will use
	m_mesh->Bind();
	m_shader->Bind();

	glm::mat4 pvmMat = projViewMat * m_modelMat;

	//Prepare the object for drawing
	BindTexture(0);

	//Set object specific uniforms
	m_shader->SetUniformMat4f("u_PVM", pvmMat);

	//Draw the object
	GLCall(glDrawElements(GL_TRIANGLES, m_mesh->GetindicesCount(), GL_UNSIGNED_INT, static_cast<void*>(0)));

	Mesh::Unbind();
	Shader::Unbind();
}

void PhysicsBox::BindTexture(unsigned int _texNum) const
{
	if (_texNum == 0)
	{
		m_tex0->Bind();
	}
}