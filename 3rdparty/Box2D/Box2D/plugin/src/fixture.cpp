#include "fixture.h"
#include "body.h"
#include "box2d.h"
#include "plugin.h"
#include <Box2D/Box2D.h>

int b2FixtureWrapper::TypeId = 0;

b2FixtureWrapper::b2FixtureWrapper(b2BodyWrapper *body, b2Fixture *fixture) :
	m_body(body),
	m_fixture(fixture)
{
	m_fixture->SetUserData(this);
}

b2FixtureWrapper::~b2FixtureWrapper()
{
	if(m_body) {
		vector<b2FixtureWrapper*>::iterator itr = find(m_body->m_fixtures.begin(), m_body->m_fixtures.end(), this);
		if(itr != m_body->m_fixtures.end())
			m_body->m_fixtures.erase(itr);
	}
}

void b2FixtureWrapper::destroy()
{
	if(m_fixture)
	{
		b2d->destroyFixture(m_body->m_body, &m_fixture); // This function will set m_fixture to 0 once the fixture is removed
		//m_fixture = 0;  // We cannot do this as it will hinder the 'end contact' call from getting called
	}
}

void b2FixtureWrapper::setFriction(const float friction)
{
	if(!m_fixture)
		return;

	m_fixture->SetFriction(friction);
}

void b2FixtureWrapper::setDensity(const float density)
{
	if(!m_fixture)
		return;

	m_fixture->SetDensity(density);
	m_fixture->GetBody()->ResetMassData();
}

void b2FixtureWrapper::setRestitution(const float restitution)
{
	if(!m_fixture)
		return;

	m_fixture->SetRestitution(restitution);
}

void b2FixtureWrapper::setSensor(const bool sensor)
{
	if(!m_fixture)
		return;

	m_fixture->SetSensor(sensor);
}

bool b2FixtureWrapper::isSensor() const
{
	if(!m_fixture)
		return false;

	return m_fixture->IsSensor();
}

void b2FixtureWrapper::setMaskBits(const uint maskBits)
{
	if(!m_fixture)
		return;

	b2Filter data = m_fixture->GetFilterData();
	data.maskBits = maskBits;
	m_fixture->SetFilterData(data);
}

void b2FixtureWrapper::setCategoryBits(const uint categoryBits)
{
	if(!m_fixture)
		return;

	b2Filter data = m_fixture->GetFilterData();
	data.categoryBits = categoryBits;
	m_fixture->SetFilterData(data);
}

b2BodyWrapper *b2FixtureWrapper::getBody() const
{
	if(!m_fixture || !m_body)
		return 0;

	m_body->addRef();
	return m_body;
}

void b2FixtureWrapper::enumReferences(asIScriptEngine *engine)
{
	if(m_body) {
		engine->GCEnumCallback(m_body);
	}
}

void b2FixtureWrapper::releaseReferences(asIScriptEngine *engine)
{
	if(m_body) {
		m_body->release();
	}
}