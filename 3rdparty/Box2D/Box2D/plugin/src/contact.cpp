#include "contact.h"
#include "box2d.h"
#include "body.h"
#include "fixture.h"
#include "plugin.h"
#include <Box2D/Box2D.h>

b2ContactWrapper::b2ContactWrapper(b2Contact *contact, b2FixtureWrapper *fixtureA, b2FixtureWrapper *fixtureB) :
	m_fixtureA(fixtureA),
	m_fixtureB(fixtureB),
	m_contact(contact)
{
	m_fixtureA->addRef();
	m_fixtureB->addRef();
}

b2ContactWrapper::~b2ContactWrapper()
{
	m_fixtureA->release();
	m_fixtureB->release();
}

void b2ContactWrapper::setEnabled(bool enabled)
{
	m_contact->SetEnabled(enabled);
}

bool b2ContactWrapper::isTouching() const
{
	return m_contact->IsTouching();
}

void b2ContactWrapper::setFriction(float friction)
{
	m_contact->SetFriction(friction);
}

float b2ContactWrapper::getFriction() const
{
	return m_contact->GetFriction();
}

void b2ContactWrapper::resetFriction()
{
	m_contact->ResetFriction();
}

void b2ContactWrapper::setRestitution(float restitution)
{
	m_contact->SetRestitution(restitution);
}

float b2ContactWrapper::getRestitution() const
{
	return m_contact->GetRestitution();
}

void b2ContactWrapper::resetRestitution()
{
	m_contact->ResetRestitution();
}

void b2ContactWrapper::swapAB()
{
	b2FixtureWrapper *tmpFixture = m_fixtureA;
	m_fixtureA = m_fixtureB;
	m_fixtureB = tmpFixture;
}

void b2ContactWrapper::call(ContactType type)
{
	// Make sure the contact is valid
	if(!isValid()) {
		return;
	}

	b2BodyWrapper *bodyA = m_fixtureA->getBody();
	asIScriptFunction *func = 0;
	switch(type)
	{
	case BeginContact: func = bodyA->m_beginContactFunc; break;
	case EndContact: func = bodyA->m_endContactFunc; break;
	case PreSolve: func = bodyA->m_preSolveFunc; break;
	case PostSolve: func = bodyA->m_postSolveFunc; break;
	}

	if(func)
	{
		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0); addRef();
		r = ctx->SetArgAddress(0, this); assert(r >= 0);
		r = ctx->Execute(); assert(r >= 0);
		r = ctx->Release(); assert(r >= 0);
	}
	bodyA->release();
}

bool b2ContactWrapper::isValid() const
{
	b2BodyWrapper *bodyA = getBodyA();
	if(!bodyA) {
		return false;
	}
	bodyA->release();

	b2BodyWrapper *bodyB = getBodyB();
	if(!bodyB) {
		return false;
	}
	bodyB->release();

	return bodyA->isValid() && bodyB->isValid();
}

b2BodyWrapper *b2ContactWrapper::getBodyA() const
{
	return m_fixtureA->getBody();
}

b2BodyWrapper *b2ContactWrapper::getBodyB() const
{
	return m_fixtureB->getBody();
}

b2FixtureWrapper *b2ContactWrapper::getFixtureA() const
{
	m_fixtureA->addRef();
	return m_fixtureA;
}

b2FixtureWrapper *b2ContactWrapper::getFixtureB() const
{
	m_fixtureB->addRef();
	return m_fixtureB;
}