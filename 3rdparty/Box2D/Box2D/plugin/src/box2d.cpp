#include "box2d.h"
#include "debugdraw.h"
#include "contactlistener.h"

Box2D *b2d = 0;

b2Vec2 toB2Vec(const Vector2 &vec)
{
	return b2Vec2(vec.x/b2d->getScale(), vec.y/b2d->getScale());
}

Vector2 toXDVec(const b2Vec2 &vec)
{
	return Vector2(vec.x*b2d->getScale(), vec.y*b2d->getScale());
}

Box2D::Box2D() :
	m_scale(1.0f)
{
	m_world = new b2World(b2Vec2(0.0f, 10.0f));
	m_world->SetAllowSleeping(true);

	LOG("Initializing Box2D %i.%i.%i", b2_version.major, b2_version.minor, b2_version.revision);
		
	uint drawFlags = b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit;
	m_debugDraw = new DebugDraw;
	m_debugDraw->SetFlags(drawFlags);
	m_world->SetDebugDraw(m_debugDraw);

	m_contactListener = new ContactListener;
	m_world->SetContactListener(m_contactListener);
}

Box2D::~Box2D()
{
	delete m_world;
	delete m_debugDraw;
	delete m_contactListener;
}

void Box2D::step(float timeStep)
{
	// Destroy bodies and fixtures while the world is unlocked
	for(set<pair<b2Body*, b2Fixture**>>::iterator itr = m_fixturesToDestroy.begin(); itr != m_fixturesToDestroy.end(); ++itr) {
		(*itr).first->DestroyFixture(*(*itr).second);
		*(*itr).second = 0; // Mark fixture as removed

	}
	m_fixturesToDestroy.clear();

	for(set<b2Body**>::iterator itr = m_bodiesToDestroy.begin(); itr != m_bodiesToDestroy.end(); ++itr) {
		m_world->DestroyBody(**itr);
		*(*itr) = 0; // Mark body as removed
	}
	m_bodiesToDestroy.clear();

	for(set<pair<b2Joint**, b2JointDef*>>::iterator itr = m_jointsToCreate.begin(); itr != m_jointsToCreate.end(); ++itr) {
		/**(*itr).first = */m_world->CreateJoint((*itr).second);
		//delete (*itr).second;
	}
	m_jointsToCreate.clear();

	m_world->Step(timeStep, 8, 3);
}

void Box2D::draw(XBatch *batch)
{
	m_drawBatch = batch;
	m_world->DrawDebugData();
}

void Box2D::setDrawFlags(int flags)
{
	m_debugDraw->SetFlags(flags);
}

void Box2D::setScale(float scale)
{
	m_scale = scale;
}

float Box2D::getScale() const
{
	return m_scale;
}

void Box2D::setGravity(const Vector2 &gravity)
{
	m_world->SetGravity(toB2Vec(gravity));
}

Vector2 Box2D::getGravity() const
{
	return toXDVec(m_world->GetGravity());
}

bool Box2D::isLocked() const
{
	return m_world->IsLocked();
}

b2World *Box2D::getWorld() const
{
	return m_world;
}

XBatch *Box2D::getDrawBatch() const
{
	return m_drawBatch;
}

void Box2D::createJoint(b2Joint **joint, const b2JointDef &def)
{
	m_jointsToCreate.insert(make_pair(joint, new b2JointDef(def)));
}

void Box2D::destroyBody(b2Body **body)
{
	m_bodiesToDestroy.insert(body);
}

void Box2D::destroyFixture(b2Body *body, b2Fixture **fixture)
{
	m_fixturesToDestroy.insert(make_pair(body, fixture));
}