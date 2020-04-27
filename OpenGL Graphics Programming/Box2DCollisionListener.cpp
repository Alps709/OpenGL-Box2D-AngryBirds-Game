#include "Box2DCollisionListener.h"
#include "AngryBoid.h"
#include "Piggie.h"
#include "Math.h"

void Box2DCollisionListener::BeginContact(b2Contact* _contact)
{
	auto tempA = _contact->GetFixtureA()->GetBody();
	auto tempB = _contact->GetFixtureB()->GetBody();

	//Convert the tag to a string pointer, if it's not null get the tag string
	PhysicsObjectData* tagA = (PhysicsObjectData*)tempA->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempA->GetUserData();
	PhysicsObjectData* tagB = (PhysicsObjectData*)tempB->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempB->GetUserData();

	if (tagA && tagB)
	{
		std::string tagAS = tagA->m_tag;
		std::string tagBS = tagB->m_tag;

		if (tagAS == "Piggie")
		{
			std::cout << "Hit Piggie" << std::endl;
		}
		if (tagBS == "Piggie")
		{
			std::cout << "Hit Piggie" << std::endl;
		}

		if (tagAS == "AngryBoid" && tagBS == "Piggie")
		{
			std::cout << "Piggie killed!" << std::endl;
			*(tagB->m_ptrToDrawnTex) = tagB->m_tex1;
		}
		else if (tagAS == "Piggie" && tagBS == "AngryBoid")
		{
			std::cout << "Piggie killed!" << std::endl;
			*(tagA->m_ptrToDrawnTex) = tagA->m_tex1;
		}
	}
	//tempB->ApplyLinearImpulseToCenter(Math::Vec2toBox2D(glm::vec2(0.0f, 100.0f * tempB->GetMass())), true);
}

void Box2DCollisionListener::EndContact(b2Contact* _contact)
{
	//std::cout << "End contact!" << std::endl;
}

void Box2DCollisionListener::PreSolve(b2Contact* _contact, const b2Manifold* _oldManifold)
{
	//std::cout << "Pre solve contact!" << std::endl;
}

void Box2DCollisionListener::PostSolve(b2Contact* _contact, const b2ContactImpulse* _impulse)
{
	auto tempA = _contact->GetFixtureA()->GetBody();
	auto tempB = _contact->GetFixtureB()->GetBody();

	//Convert the tag to a string pointer, if it's not null get the tag string
	PhysicsObjectData* tagA = (PhysicsObjectData*)tempA->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempA->GetUserData();
	PhysicsObjectData* tagB = (PhysicsObjectData*)tempB->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempB->GetUserData();

	if (tagA && tagB)
	{
		std::string tagAS = tagA->m_tag;
		std::string tagBS = tagB->m_tag;

		if (tagAS == "Box" && tagBS == "Piggie")
		{
			if (_impulse->normalImpulses[0] > 2.5f)
			{
				std::cout << "Piggie killed!" << std::endl;

				*(tagB->m_ptrToDrawnTex) = tagB->m_tex1;
			}
		}
		else if (tagAS == "Piggie" && tagBS == "Box")
		{
			if (_impulse->normalImpulses[0] > 2.5f)
			{
				std::cout << "Piggie killed!" << std::endl;

				*(tagA->m_ptrToDrawnTex) = tagA->m_tex1;
			}
		}
	}
}
