#include "Box2DCollisionListener.h"
#include "AngryBoid.h"
#include "Piggie.h"
#include "Math.h"

void Box2DCollisionListener::BeginContact(b2Contact* _contact)
{
	auto tempA = _contact->GetFixtureA()->GetBody();
	auto tempB = _contact->GetFixtureB()->GetBody();

	//Convert the user data from a void*, if it's not null then get the user data
	PhysicsObjectData* tagA = (PhysicsObjectData*)tempA->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempA->GetUserData();
	PhysicsObjectData* tagB = (PhysicsObjectData*)tempB->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempB->GetUserData();

	if (tagA && tagB)
	{
		std::string tagAS = tagA->m_tag;
		std::string tagBS = tagB->m_tag;

		if (tagAS == "AngryBoid" && tagBS == "Piggie")
		{
			//Set the drawn texture of the piggie to be the dead piggie
			*(tagB->m_ptrToDrawnTex) = tagB->m_tex1;
			//Set bool to tell that piggie died
			*(tagB->m_hasDied) = true;
		}
		else if (tagAS == "Piggie" && tagBS == "AngryBoid")
		{
			//Set the drawn texture of the piggie to be the dead piggie
			*(tagA->m_ptrToDrawnTex) = tagA->m_tex1;
			//Set bool to tell that piggie died
			*(tagA->m_hasDied) = true;
		}
	}
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
	PhysicsObjectData* dataA = (PhysicsObjectData*)tempA->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempA->GetUserData();
	PhysicsObjectData* dataB = (PhysicsObjectData*)tempB->GetUserData() == nullptr ? nullptr : (PhysicsObjectData*)tempB->GetUserData();

	//Handle destruction of destructable box
	if (dataA)
	{
		std::string tagDataA = dataA->m_tag;

		
		if (tagDataA == "Box" && dataA->m_destructable)
		{
			//If a destructable box hits anything with an impulse greater than 2.5, then the box is destroyed
			if (_impulse->normalImpulses[0] > 3.5f)
			{
				//Change the drawn texture to the destroyed box texture
				*(dataA->m_ptrToDrawnTex) = dataA->m_tex1;

				//Set bool to tell that box was destroyed
				*(dataA->m_hasDied) = true;
			}
		}
	}

	if (dataB)
	{
		std::string tagDataB = dataB->m_tag;

		//Handle destruction of destructable boxes
		if (tagDataB == "Box" && dataB->m_destructable)
		{
			//If a destructable box hits anything with an impulse greater than 2.5, then the box is destroyed
			if (_impulse->normalImpulses[0] > 3.5f)
			{
				//Change the drawn texture to the destroyed box texture
				*(dataB->m_ptrToDrawnTex) = dataB->m_tex1;

				//Set bool to tell that box was destroyed
				*(dataB->m_hasDied) = true;
			}
		}
	}

	if (dataA && dataB)
	{
		std::string tagDataA = dataA->m_tag;
		std::string tagDataB = dataB->m_tag;

		//Handle collisions between box and piggie
		if (tagDataA == "Box" && tagDataB == "Piggie")
		{
			//If a box hits a piggie with an impulse greater than 2.5, then the box kills the piggie :(
			if (_impulse->normalImpulses[0] > 2.5f)
			{
				//Change the drawn texture to the dead piggie texture
				*(dataB->m_ptrToDrawnTex) = dataB->m_tex1;

				//Set bool to tell that piggie died
				*(dataB->m_hasDied) = true;
			}
		}
		else if (tagDataA == "Piggie" && tagDataB == "Box")
		{
			//If a box hits a piggie with an impulse greater than 2.5, then the box kills the piggie :(
			if (_impulse->normalImpulses[0] > 2.5f)
			{
				//Set the drawn texture of the piggie to be the dead piggie
				*(dataA->m_ptrToDrawnTex) = dataA->m_tex1;

				//Set bool to tell that piggie died
				*(dataA->m_hasDied) = true;
			}
		}
	}
}
