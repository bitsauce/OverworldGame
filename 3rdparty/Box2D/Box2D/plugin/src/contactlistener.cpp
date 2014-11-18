#include "contactlistener.h"
#include "contact.h"
#include "body.h"

void ContactListener::BeginContact(b2Contact* b2c)
{
	b2ContactWrapper *contact = new b2ContactWrapper(b2c,
		(b2FixtureWrapper*)b2c->GetFixtureA()->GetUserData(),
		(b2FixtureWrapper*)b2c->GetFixtureB()->GetUserData());
	contact->call(b2ContactWrapper::BeginContact);
	contact->swapAB();
	contact->call(b2ContactWrapper::BeginContact);
	contact->release();
}

void ContactListener::EndContact(b2Contact* b2c)
{
	b2ContactWrapper *contact = new b2ContactWrapper(b2c,
		(b2FixtureWrapper*)b2c->GetFixtureA()->GetUserData(),
		(b2FixtureWrapper*)b2c->GetFixtureB()->GetUserData());
	contact->call(b2ContactWrapper::EndContact);
	contact->swapAB();
	contact->call(b2ContactWrapper::EndContact);
	contact->release();
}

void ContactListener::PreSolve(b2Contact* b2c, const b2Manifold* oldManifold)
{
	b2ContactWrapper *contact = new b2ContactWrapper(b2c,
		(b2FixtureWrapper*)b2c->GetFixtureA()->GetUserData(),
		(b2FixtureWrapper*)b2c->GetFixtureB()->GetUserData());
	contact->call(b2ContactWrapper::PreSolve);
	contact->swapAB();
	contact->call(b2ContactWrapper::PreSolve);
	contact->release();
}

void ContactListener::PostSolve(b2Contact* b2c, const b2ContactImpulse* impulse)
{
	b2ContactWrapper *contact = new b2ContactWrapper(b2c,
		(b2FixtureWrapper*)b2c->GetFixtureA()->GetUserData(),
		(b2FixtureWrapper*)b2c->GetFixtureB()->GetUserData());
	contact->call(b2ContactWrapper::PostSolve);
	contact->swapAB();
	contact->call(b2ContactWrapper::PostSolve);
	contact->release();
}