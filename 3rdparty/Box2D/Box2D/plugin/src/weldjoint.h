#ifndef PLUGIN_WELD_JOINT_H
#define PLUGIN_WELD_JOINT_H

#include "box2d.h"

class b2WeldJointDefWrapper
{
public:
	b2WeldJointDefWrapper() :
		bodyA(0),
		bodyB(0),
		anchor(0.0f),
		localAnchorA(0.0f),
		localAnchorB(0.0f),
		referenceAngle(0.0f),
		frequencyHz(0.0f),
		dampingRatio(0.0f),
		collideConnected(false)
	{
	}

	~b2WeldJointDefWrapper()
	{
		setBodyA(0);
		setBodyB(0);
	}

	void initialize(b2BodyWrapper *bodyA, b2BodyWrapper *bodyB, Vector2 anchor);

	b2WeldJointDef toB2() const;
	
	void setBodyA(b2BodyWrapper *body);
	b2BodyWrapper *getBodyA() const;
	void setBodyB(b2BodyWrapper *body);
	b2BodyWrapper *getBodyB() const;
	Vector2 anchor;
	Vector2 localAnchorA;
	Vector2 localAnchorB;
	float referenceAngle;
	float frequencyHz;
	float dampingRatio;
	bool collideConnected;
	
	static void Construct(b2WeldJointDefWrapper *self) { new (self) b2WeldJointDefWrapper; }
	static void Destruct(b2WeldJointDefWrapper *self) { self->~b2WeldJointDefWrapper(); }

private:
	b2BodyWrapper *bodyA;
	b2BodyWrapper *bodyB;
};

class b2WeldJointWrapper
{
public:
	b2WeldJointWrapper();
	
	float getReferenceAngle() const;

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	static b2WeldJointWrapper *Factory(const b2WeldJointDefWrapper&);

private:
	b2WeldJoint *m_self;
};

#endif // PLUGIN_WELD_JOINT_H