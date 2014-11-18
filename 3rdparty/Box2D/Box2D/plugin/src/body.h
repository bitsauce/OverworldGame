#ifndef B2_BODY_WRAPPER_H
#define B2_BODY_WRAPPER_H

#include <x2d/x2d.h>
#include <Box2D/Dynamics/b2Body.h>

class b2Body;
class b2FixtureWrapper;

enum b2BodyTypeWrapper
{
	StaticBody = b2_staticBody,
	KinematicBody = b2_kinematicBody,
	DynamicBody = b2_dynamicBody,
	BulletBody
};

class b2BodyDefWrapper
{
public:
	b2BodyDefWrapper();
	b2BodyDef getBodyDef() const;

	b2BodyTypeWrapper type;
	Vector2 position;
	float angle;
	Vector2 linearVelocity;
	float angularVelocity;
	float linearDamping;
	float angularDamping;
	bool allowSleep;
	bool awake;
	bool fixedRotation;
	bool active;
	float gravityScale;

	static void Construct(b2BodyDefWrapper *self) { new (self) b2BodyDefWrapper; }
};

class b2BodyWrapper
{
	friend class b2FixtureWrapper;
	friend class b2ContactWrapper;
public:
	b2BodyWrapper(b2Body *body);
	~b2BodyWrapper();
	void destroy();

	void addRef()
	{
		gcFlag = false;
		refCounter.add();
	}
	void release()
	{
		gcFlag = false;
		if(refCounter.release() == 0) {
			delete this;
		}
	}
	int getRefCount() { return refCounter.get(); }
	void setGCFlag() { gcFlag = true; }
	bool getGCFlag() { return gcFlag; }
	void enumReferences(asIScriptEngine*);
	void releaseReferences(asIScriptEngine*);

	b2FixtureWrapper *createFixture(const Rect &rect, float density);
	b2FixtureWrapper *createFixture(const Vector2 &center, const float radius, float density);
	b2FixtureWrapper *createFixture(XScriptArray *arr, float density);

	void removeFixture(b2FixtureWrapper *fixture);

	void setTransform(const Vector2 &position, float angle);

	void setPosition(const Vector2 &position);

	void setAngle(float angle);

	void setActive(const bool active);
	bool isActive() const;

	void setBeginContactCallback(asIScriptFunction *func);

	void setEndContactCallback(asIScriptFunction *func);

	void setPreSolveCallback(asIScriptFunction *func);

	void setPostSolveCallback(asIScriptFunction *func);

	void setObject(void *object, int typeId);

	void freeObject();
	bool getObject(void *object, int typeId);

	Vector2 getPosition() const;
	Vector2 getLocalPoint(const Vector2 &worldPoint) const;


	float getAngle() const;
	Vector2 getCenter() const;
	Vector2 getLinearVelocity() const;
	float getMass() const;
	void applyImpulse(const Vector2 &impulse, const Vector2 &position);
	void setLinearVelocity(const Vector2 &velocity);
	bool isValid() const;

	b2Body *toB2() const {
		return m_body;
	}
	
	static b2BodyWrapper *Factory(const b2BodyDefWrapper &def);
	static int TypeId;

private:
	RefCounter refCounter;
	bool gcFlag;
	b2Body *m_body;
	vector<b2FixtureWrapper*> m_fixtures;
	struct
    {
		void *value;
		int   typeId;
    } m_object;
	asIScriptFunction *m_beginContactFunc;
	asIScriptFunction *m_endContactFunc;
	asIScriptFunction *m_preSolveFunc;
	asIScriptFunction *m_postSolveFunc;
};

#endif // B2_BODY_WRAPPER_H