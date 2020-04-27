#pragma once
#include <box2d.h>

class Box2DCollisionListener : public b2ContactListener
{
	virtual void BeginContact(b2Contact* _contact) override;
	virtual void EndContact(b2Contact* _contact) override;
	virtual void PreSolve(b2Contact* _contact, const b2Manifold* _oldManifold) override;
	virtual void PostSolve(b2Contact* _contact, const b2ContactImpulse* _impulse) override;
};
