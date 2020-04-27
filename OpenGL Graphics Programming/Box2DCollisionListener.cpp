#include "Box2DCollisionListener.h"
#include "AngryBoid.h"
#include "Piggie.h"
#include "Math.h"

void Box2DCollisionListener::BeginContact(b2Contact* _contact)
{
	auto tempA = _contact->GetFixtureA()->GetBody();
	auto tempB = _contact->GetFixtureB()->GetBody();

	//Convert the tag to a string pointer, if it's not null get the tag string
	PhysicsCircle* tagA = (PhysicsCircle*)tempA->GetUserData() == nullptr ? nullptr : (PhysicsCircle*)tempA->GetUserData();
	PhysicsCircle* tagB = (PhysicsCircle*)tempB->GetUserData() == nullptr ? nullptr : (PhysicsCircle*)tempA->GetUserData();

	if (tagA && tagB)
	{
		bool safe = true;
		try
		{
			tagA->GetTag();
			tagB->GetTag();
		}
		catch (...)
		{
			safe = false;
		}

		if (safe)
		{
			if (tagA->GetTag() == "AngryBoid" && tagB->GetTag() == "Piggie")
			{
				std::cout << "Piggie killed!" << std::endl;
				tagB->SetDrawnTex(1);
			}
			else if (tagB->GetTag() == "AngryBoid" && tagA->GetTag() == "Piggie")
			{
				std::cout << "Piggie killed!" << std::endl;
				tagA->SetDrawnTex(1);
			}
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
	//std::cout << "Post solve contact!" << std::endl;
}
