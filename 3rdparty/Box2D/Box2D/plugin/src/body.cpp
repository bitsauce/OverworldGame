#include "body.h"
#include "fixture.h"
#include "box2d.h"
#include "plugin.h"
#include <Box2D/Box2D.h>

int b2BodyWrapper::TypeId = 0;

b2BodyDefWrapper::b2BodyDefWrapper() :
	type(StaticBody),
	position(0.0f),
	angle(0.0f),
	linearVelocity(0.0f),
	angularVelocity(0.0f),
	linearDamping(0.0f),
	angularDamping(0.0f),
	allowSleep(true),
	awake(true),
	fixedRotation(false),
	active(true),
	gravityScale(1.0f)
{
}

b2BodyDef b2BodyDefWrapper::getBodyDef() const
{
	b2BodyDef def;
	def.type = (type != BulletBody ? b2BodyType(type) : b2_dynamicBody);
	def.position = toB2Vec(position);
	def.angle = angle;
	def.linearVelocity = toB2Vec(linearVelocity);
	def.angularVelocity = angularVelocity;
	def.linearDamping = linearDamping;
	def.angularDamping = angularDamping;
	def.allowSleep = allowSleep;
	def.awake = awake;
	def.fixedRotation = fixedRotation;
	def.bullet = (type == BulletBody);
	def.active = active;
	def.gravityScale = gravityScale;
	return def;
}

b2BodyWrapper::b2BodyWrapper(b2Body *body) :
	m_body(body),
	m_beginContactFunc(0),
	m_endContactFunc(0),
	m_preSolveFunc(0),
	m_postSolveFunc(0)
{
	m_object.value = 0;
	m_object.typeId = 0;
	m_body->SetUserData(this);
}

b2BodyWrapper::~b2BodyWrapper()
{
	freeObject();
	destroy();
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		(*itr)->m_body = 0;
	}
}

void b2BodyWrapper::destroy()
{
	if(m_body)
	{
		b2d->destroyBody(&m_body); // This function will set m_body to 0 once the body is removed
		for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
			(*itr)->destroy();
		}
		//m_body = 0; // We cannot do this as it will hinder the 'end contact' call from getting called
	}
}

b2FixtureWrapper *b2BodyWrapper::createFixture(const Rect &rect, float density)
{
	b2FixtureWrapper *fixture = 0;
	if(isValid() && !b2d->isLocked())
	{
		// Setup fixture def
		b2PolygonShape shape;
		b2Vec2 halfSize = toB2Vec(rect.getSize()/2.0f);
		shape.SetAsBox(halfSize.x, halfSize.y, toB2Vec(rect.getCenter()), 0.0f);
	
		// Create fixture
		addRef(); // Add a ref for the fixture to hold
		fixture = new b2FixtureWrapper(this, m_body->CreateFixture(&shape, density));
		fixture->addRef(); // Add a reference for the body to hold
	
		// Notify garbage collector of the newly created object
		XScriptEngine::GetAngelScript()->NotifyGarbageCollectorOfNewObject(fixture, XScriptEngine::GetAngelScript()->GetObjectTypeById(b2FixtureWrapper::TypeId));

		// Add fixture to list of fixtures owned by this body
		m_fixtures.push_back(fixture);
	}
	return fixture;
}
	
b2FixtureWrapper *b2BodyWrapper::createFixture(const Vector2 &center, const float radius, float density)
{
	b2FixtureWrapper *fixture = 0;
	if(isValid() && !b2d->isLocked())
	{
		// Setup fixture def
		b2CircleShape shape;
		shape.m_p = toB2Vec(center);
		shape.m_radius = radius/b2d->getScale();
	
		// Create fixture
		addRef(); // Add a ref for the fixture to hold
		fixture = new b2FixtureWrapper(this, m_body->CreateFixture(&shape, density));
		fixture->addRef(); // Add a reference for the body to hold
	
		// Notify garbage collector of the newly created object
		XScriptEngine::GetAngelScript()->NotifyGarbageCollectorOfNewObject(fixture, XScriptEngine::GetAngelScript()->GetObjectTypeById(b2FixtureWrapper::TypeId));
		
		// Add fixture to list of fixtures owned by this body
		m_fixtures.push_back(fixture);
	}
	return fixture;
}
	
b2FixtureWrapper *b2BodyWrapper::createFixture(XScriptArray *arr, float density)
{
	b2FixtureWrapper *fixture = 0;
	if(isValid() && !b2d->isLocked() && arr->GetSize() <= b2_maxPolygonVertices)
	{
		// Set vertex count
		b2Vec2 *verts = new b2Vec2[arr->GetSize()];
		for(uint i = 0; i < arr->GetSize(); i++)
		{
			verts[i] = toB2Vec(*(Vector2*)arr->At(i));
		}

		// Set shape
		b2PolygonShape shape;
		shape.Set(verts, arr->GetSize());
		delete[] verts;
		
		// Create fixture
		addRef(); // Add a ref for the fixture to hold
		fixture = new b2FixtureWrapper(this, m_body->CreateFixture(&shape, density));
		fixture->addRef(); // Add a reference for the body to hold
	
		// Notify garbage collector of the newly created object
		XScriptEngine::GetAngelScript()->NotifyGarbageCollectorOfNewObject(fixture, XScriptEngine::GetAngelScript()->GetObjectTypeById(b2FixtureWrapper::TypeId));
		
		// Add fixture to list of fixtures owned by this body
		m_fixtures.push_back(fixture);
	}
	return fixture;
}

void b2BodyWrapper::removeFixture(b2FixtureWrapper *fixture)
{
	if(isValid() && !b2d->isLocked())
	{
		// Setup fixture def
		vector<b2FixtureWrapper*>::iterator itr;
		if((itr = find(m_fixtures.begin(), m_fixtures.end(), fixture)) != m_fixtures.end())
		{
			fixture->destroy();
			//fixture->release();
			//m_fixtures.erase(itr);
		}
		fixture->release();
	}
}

void b2BodyWrapper::setTransform(const Vector2 &position, float angle)
{
	if(isValid() && !b2d->isLocked())
	{
		m_body->SetTransform(toB2Vec(position), angle);
	}
}

void b2BodyWrapper::setPosition(const Vector2 &position)
{
	if(isValid() && !b2d->isLocked())
	{
		m_body->SetTransform(toB2Vec(position), m_body->GetAngle());
	}
}

void b2BodyWrapper::setAngle(float angle)
{
	if(isValid() && !b2d->isLocked())
	{
		m_body->SetTransform(m_body->GetPosition(), angle);
	}
}

void b2BodyWrapper::setActive(const bool active)
{
	if(isValid() && !b2d->isLocked())
	{
		m_body->SetActive(active);
	}
}

bool b2BodyWrapper::isActive() const
{
	return isValid() ? m_body->IsActive() : false;
}

void b2BodyWrapper::setBeginContactCallback(asIScriptFunction *func)
{
	m_beginContactFunc = func;
}

void b2BodyWrapper::setEndContactCallback(asIScriptFunction *func)
{
	m_endContactFunc = func;
}

void b2BodyWrapper::setPreSolveCallback(asIScriptFunction *func)
{
	m_preSolveFunc = func;
}

void b2BodyWrapper::setPostSolveCallback(asIScriptFunction *func)
{
	m_postSolveFunc = func;
}

void b2BodyWrapper::freeObject()
{
	// If it is a handle or a ref counted object, call release
	if(m_object.typeId & asTYPEID_MASK_OBJECT)
	{
		// Let the engine release the object
		asIObjectType *ot = XScriptEngine::GetAngelScript()->GetObjectTypeById(m_object.typeId);
		XScriptEngine::GetAngelScript()->ReleaseScriptObject(m_object.value, ot);

		// Release the object type info
		if(ot) ot->Release();

		m_object.value = 0;
		m_object.typeId = 0;
	}
}

void b2BodyWrapper::setObject(void *object, int typeId)
{
	// Hold on to the object type reference so it isn't destroyed too early
	if(*(void**)object && (typeId & asTYPEID_MASK_OBJECT))
	{
		asIObjectType *ot = XScriptEngine::GetAngelScript()->GetObjectTypeById(typeId);
		if(ot) ot->AddRef();
	}

	freeObject();

	m_object.typeId = typeId;
	if(typeId & asTYPEID_OBJHANDLE)
	{
		// We're receiving a reference to the handle, so we need to dereference it
		m_object.value = *(void**)object;
		XScriptEngine::GetAngelScript()->AddRefScriptObject(m_object.value, XScriptEngine::GetAngelScript()->GetObjectTypeById(typeId));
	}
	else
	{
		LOG("b2Body.setObject: Supports only object handles!");
	}
}

bool b2BodyWrapper::getObject(void *object, int typeId)
{
	if(typeId & asTYPEID_OBJHANDLE)
	{
		// Is the handle type compatible with the stored value?

		// A handle can be retrieved if the stored type is a handle of same or compatible type
		// or if the stored type is an object that implements the interface that the handle refer to.
		if((m_object.typeId & asTYPEID_MASK_OBJECT) && XScriptEngine::GetAngelScript()->IsHandleCompatibleWithObject(m_object.value, m_object.typeId, typeId))
		{
			XScriptEngine::GetAngelScript()->AddRefScriptObject(m_object.value, XScriptEngine::GetAngelScript()->GetObjectTypeById(m_object.typeId));
			*(void**)object = m_object.value;

			return true;
		}
	}
	return false;
}

void b2BodyWrapper::enumReferences(asIScriptEngine *engine)
{
	// If we're holding a reference, we'll notify the garbage collector of it
	if(m_object.value && (m_object.typeId & asTYPEID_MASK_OBJECT))
	{
		engine->GCEnumCallback(m_object.value);

		// The object type itself is also garbage collected
		asIObjectType *ot = engine->GetObjectTypeById(m_object.typeId);
		if(ot) engine->GCEnumCallback(ot);
	}
	if(m_beginContactFunc) {
		engine->GCEnumCallback(m_beginContactFunc);
	}
	if(m_endContactFunc) {
		engine->GCEnumCallback(m_endContactFunc);
	}
	if(m_preSolveFunc) {
		engine->GCEnumCallback(m_preSolveFunc);
	}
	if(m_postSolveFunc) {
		engine->GCEnumCallback(m_postSolveFunc);
	}
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		engine->GCEnumCallback(*itr);
	}
}

void b2BodyWrapper::releaseReferences(asIScriptEngine *engine)
{
	// Release all held references
	freeObject();
	if(m_beginContactFunc) {
		m_beginContactFunc->Release();
	}
	if(m_endContactFunc) {
		m_endContactFunc->Release();
	}
	if(m_preSolveFunc) {
		m_preSolveFunc->Release();
	}
	if(m_postSolveFunc) {
		m_postSolveFunc->Release();
	}
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		(*itr)->release();
	}
}

Vector2 b2BodyWrapper::getPosition() const
{
	return isValid() ? toXDVec(m_body->GetPosition()) : Vector2(0.0f);
}

Vector2 b2BodyWrapper::getLocalPoint(const Vector2 &worldPoint) const
{
	return isValid() ? toXDVec(m_body->GetLocalPoint(toB2Vec(worldPoint))) : Vector2(0.0f);
}

float b2BodyWrapper::getAngle() const
{
	return isValid() ? m_body->GetAngle() : 0.0f;
}

Vector2 b2BodyWrapper::getCenter() const
{
	return isValid() ? toXDVec(m_body->GetWorldCenter()) : Vector2(0.0f);
}

Vector2 b2BodyWrapper::getLinearVelocity() const
{
	return isValid() ? toXDVec(m_body->GetLinearVelocity()) : Vector2(0.0f);
}

float b2BodyWrapper::getMass() const
{
	return isValid() ? m_body->GetMass() : 0.0f;
}

void b2BodyWrapper::applyImpulse(const Vector2 &impulse, const Vector2 &position)
{
	if(isValid())
	{
		m_body->ApplyLinearImpulse(toB2Vec(impulse), toB2Vec(position), true);
	}
}

void b2BodyWrapper::setLinearVelocity(const Vector2 &velocity)
{
	if(isValid())
	{
		m_body->SetLinearVelocity(toB2Vec(velocity));
	}
}

bool b2BodyWrapper::isValid() const
{
	return m_body != 0;
}

b2BodyWrapper *b2BodyWrapper::Factory(const b2BodyDefWrapper &def)
{
	b2BodyWrapper *body = 0;

	// Check if world is locked
	if(!b2d->isLocked())
	{
		// Create b2Body
		body = new b2BodyWrapper(b2d->getWorld()->CreateBody(&def.getBodyDef()));

		// Notify garbage collector of the newly created object
		XScriptEngine::GetAngelScript()->NotifyGarbageCollectorOfNewObject(body, XScriptEngine::GetAngelScript()->GetObjectTypeById(b2BodyWrapper::TypeId));
	}
	else
	{
		// We cannot create a b2Body if the world is locked
		LOG("b2Body::b2Body() - World locked");
	}

	// Return body
	return body;
}