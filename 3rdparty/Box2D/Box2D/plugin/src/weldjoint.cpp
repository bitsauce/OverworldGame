#include "weldjoint.h"

b2WeldJointDef b2WeldJointDefWrapper::toB2() const
{
	b2WeldJointDef def;
	def.bodyA = bodyA->toB2();
	def.bodyB = bodyB->toB2();
	def.collideConnected = collideConnected;
	def.dampingRatio = dampingRatio;
	def.frequencyHz = frequencyHz;
	def.localAnchorA = toB2Vec(localAnchorA);
	def.localAnchorB = toB2Vec(localAnchorB);
	def.referenceAngle = referenceAngle;
	return def;
}

void b2WeldJointDefWrapper::initialize(b2BodyWrapper *bodyA, b2BodyWrapper *bodyB, Vector2 anchor)
{
	setBodyA(bodyA);
	setBodyB(bodyB);
	localAnchorA = bodyA->getLocalPoint(anchor);
	localAnchorB = bodyB->getLocalPoint(anchor);
	referenceAngle = bodyB->getAngle() - bodyA->getAngle();
}

void b2WeldJointDefWrapper::setBodyA(b2BodyWrapper *body)
{
	if(bodyA) {
		bodyA->release();
	}
	bodyA = body;
}

b2BodyWrapper *b2WeldJointDefWrapper::getBodyA() const
{
	if(bodyA) {
		bodyA->addRef();
	}
	return bodyA;
}

void b2WeldJointDefWrapper::setBodyB(b2BodyWrapper *body)
{
	if(bodyB) {
		bodyB->release();
	}
	bodyB = body;
}

b2BodyWrapper *b2WeldJointDefWrapper::getBodyB() const
{
	if(bodyB) {
		bodyB->addRef();
	}
	return bodyB;
}

b2WeldJointWrapper::b2WeldJointWrapper() :
	m_self(0)
{
}

float b2WeldJointWrapper::getReferenceAngle() const
{
	if(!m_self)
		return 0.0f;

	return m_self->GetReferenceAngle();
}

b2WeldJointWrapper *b2WeldJointWrapper::Factory(const b2WeldJointDefWrapper &def)
{
	b2WeldJointWrapper *joint = new b2WeldJointWrapper;
	joint->m_self = (b2WeldJoint*)b2d->getWorld()->CreateJoint(&def.toB2());
	return joint;
}