//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "plugin.h"
#include "box2d.h"
#include "body.h"
#include "fixture.h"
#include "contact.h"
#include "weldjoint.h"

#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Settings.h>

int CreatePlugin(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectType("ScriptBox2D", 0, asOBJ_REF | asOBJ_NOHANDLE); AS_ASSERT
	r = scriptEngine->RegisterObjectType("b2BodyDef", sizeof(b2BodyDefWrapper), asOBJ_VALUE | asOBJ_POD); AS_ASSERT
	r = b2FixtureWrapper::TypeId	= scriptEngine->RegisterObjectType("b2Fixture", 0, asOBJ_REF | asOBJ_GC); AS_ASSERT
	r = b2BodyWrapper::TypeId		= scriptEngine->RegisterObjectType("b2Body", 0, asOBJ_REF | asOBJ_GC); AS_ASSERT
	r = scriptEngine->RegisterObjectType("b2Contact", 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectType("b2WeldJointDef", sizeof(b2WeldJointDefWrapper), asOBJ_VALUE | asOBJ_APP_CLASS_CD); AS_ASSERT
	r = scriptEngine->RegisterObjectType("b2WeldJoint", 0, asOBJ_REF); AS_ASSERT

	r = scriptEngine->RegisterFuncdef("void b2ContactCallback(b2Contact@)"); AS_ASSERT
	
	r = scriptEngine->RegisterEnum("BodyType"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BodyType", "b2_staticBody", StaticBody); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BodyType", "b2_kinematicBody", KinematicBody); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BodyType", "b2_dynamicBody", DynamicBody); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BodyType", "b2_bulletBody", BulletBody);  AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "void step(float)", asMETHOD(Box2D, step), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "void draw(Batch @)", asMETHOD(Box2D, draw), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "void setDrawFlags(int)", asMETHOD(Box2D, setDrawFlags), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "void set_scale(float)", asMETHOD(Box2D, setScale), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "float get_scale() const", asMETHOD(Box2D, getScale), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "void set_gravity(const Vector2 &in)", asMETHOD(Box2D, setGravity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "Vector2 get_gravity() const", asMETHOD(Box2D, getGravity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptBox2D", "bool get_locked() const", asMETHOD(Box2D, isLocked), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectBehaviour("b2BodyDef", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(b2BodyDefWrapper::Construct), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "BodyType type", offsetof(b2BodyDefWrapper, type)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "Vector2 position", offsetof(b2BodyDefWrapper, position)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "float angle", offsetof(b2BodyDefWrapper, angle)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "Vector2 linearVelocity", offsetof(b2BodyDefWrapper, linearVelocity)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "float angularVelocity", offsetof(b2BodyDefWrapper, angularVelocity)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "float linearDamping", offsetof(b2BodyDefWrapper, linearDamping)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "float angularDamping", offsetof(b2BodyDefWrapper, angularDamping)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "bool allowSleep", offsetof(b2BodyDefWrapper, allowSleep)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "bool awake", offsetof(b2BodyDefWrapper, awake)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "bool fixedRotation", offsetof(b2BodyDefWrapper, fixedRotation)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "bool active", offsetof(b2BodyDefWrapper, active)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2BodyDef", "float gravityScale", offsetof(b2BodyDefWrapper, gravityScale)); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_ADDREF, "void f()", asMETHOD(b2FixtureWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_RELEASE, "void f()", asMETHOD(b2FixtureWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(b2FixtureWrapper, setGCFlag), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(b2FixtureWrapper, getGCFlag), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(b2FixtureWrapper, getRefCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(b2FixtureWrapper, enumReferences), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Fixture", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(b2FixtureWrapper, releaseReferences), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "void setDensity(const float)", asMETHOD(b2FixtureWrapper, setDensity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "void setFriction(const float)", asMETHOD(b2FixtureWrapper, setFriction), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "void setRestitution(const float)", asMETHOD(b2FixtureWrapper, setRestitution), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "void setSensor(const bool)", asMETHOD(b2FixtureWrapper, setSensor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "bool isSensor() const", asMETHOD(b2FixtureWrapper, isSensor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "void setMaskBits(const uint)", asMETHOD(b2FixtureWrapper, setMaskBits), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "void setCategoryBits(const uint)", asMETHOD(b2FixtureWrapper, setCategoryBits), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Fixture", "b2Body @getBody() const", asMETHOD(b2FixtureWrapper, getBody), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_ADDREF, "void f()", asMETHOD(b2BodyWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_RELEASE, "void f()", asMETHOD(b2BodyWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(b2BodyWrapper, setGCFlag), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(b2BodyWrapper, getGCFlag), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(b2BodyWrapper, getRefCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(b2BodyWrapper, enumReferences), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(b2BodyWrapper, releaseReferences), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_FACTORY, "b2Body @f(const b2BodyDef &in)", asFUNCTIONPR(b2BodyWrapper::Factory, (const b2BodyDefWrapper&), b2BodyWrapper*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void destroy()", asMETHOD(b2BodyWrapper, destroy), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "b2Fixture @createFixture(const Rect &in, float)", asMETHODPR(b2BodyWrapper, createFixture, (const Rect&, float), b2FixtureWrapper*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "b2Fixture @createFixture(const Vector2 &in, const float, float)", asMETHODPR(b2BodyWrapper, createFixture, (const Vector2&, const float, float), b2FixtureWrapper*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "b2Fixture @createFixture(array<Vector2> &in, float density)", asMETHODPR(b2BodyWrapper, createFixture, (XScriptArray*, float), b2FixtureWrapper*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void removeFixture(b2Fixture @)", asMETHOD(b2BodyWrapper, removeFixture), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setTransform(const Vector2 &in, float)", asMETHOD(b2BodyWrapper, setTransform), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setPosition(const Vector2 &in)", asMETHOD(b2BodyWrapper, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setAngle(float)", asMETHOD(b2BodyWrapper, setAngle), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setActive(const bool)", asMETHOD(b2BodyWrapper, setActive), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "bool isActive() const", asMETHOD(b2BodyWrapper, isActive), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setBeginContactCallback(b2ContactCallback@)", asMETHOD(b2BodyWrapper, setBeginContactCallback), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setEndContactCallback(b2ContactCallback@)", asMETHOD(b2BodyWrapper, setEndContactCallback), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setPreSolveCallback(b2ContactCallback@)", asMETHOD(b2BodyWrapper, setPreSolveCallback), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setPostSolveCallback(b2ContactCallback@)", asMETHOD(b2BodyWrapper, setPostSolveCallback), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setObject(?&in)", asMETHOD(b2BodyWrapper, setObject), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "bool getObject(?&out)", asMETHOD(b2BodyWrapper, getObject), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "Vector2 getPosition() const", asMETHOD(b2BodyWrapper, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "Vector2 getLocalPoint(const Vector2 &in) const", asMETHOD(b2BodyWrapper, getLocalPoint), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "Vector2 getCenter() const", asMETHOD(b2BodyWrapper, getCenter), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "Vector2 getLinearVelocity() const", asMETHOD(b2BodyWrapper, getLinearVelocity), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "float getMass() const", asMETHOD(b2BodyWrapper, getMass), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "float getAngle() const", asMETHOD(b2BodyWrapper, getAngle), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void applyImpulse(const Vector2 &in, const Vector2 &in)", asMETHOD(b2BodyWrapper, applyImpulse), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Body", "void setLinearVelocity(const Vector2 &in)", asMETHOD(b2BodyWrapper, setLinearVelocity), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("b2Contact", asBEHAVE_ADDREF, "void f()", asMETHOD(b2ContactWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2Contact", asBEHAVE_RELEASE, "void f()", asMETHOD(b2ContactWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "void setEnabled(bool)", asMETHOD(b2ContactWrapper, setEnabled), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "bool isTouching() const", asMETHOD(b2ContactWrapper, isTouching), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "void setFriction(float)", asMETHOD(b2ContactWrapper, setFriction), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "float getFriction() const", asMETHOD(b2ContactWrapper, getFriction), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "void resetFriction()", asMETHOD(b2ContactWrapper, resetFriction), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "void setRestitution(float)", asMETHOD(b2ContactWrapper, setRestitution), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "float getRestitution() const", asMETHOD(b2ContactWrapper, getRestitution), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "void resetRestitution()", asMETHOD(b2ContactWrapper, resetRestitution), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "b2Body @get_bodyA() const", asMETHOD(b2ContactWrapper, getBodyA), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "b2Body @get_bodyB() const", asMETHOD(b2ContactWrapper, getBodyB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "b2Fixture @get_fixtureA() const", asMETHOD(b2ContactWrapper, getFixtureA), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2Contact", "b2Fixture @get_fixtureB() const", asMETHOD(b2ContactWrapper, getFixtureB), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("b2WeldJointDef", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(b2WeldJointDefWrapper::Construct), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2WeldJointDef", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(b2WeldJointDefWrapper::Destruct), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2WeldJointDef", "void initialize(b2Body@, b2Body@, Vector2)", asMETHOD(b2WeldJointDefWrapper, initialize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2WeldJointDef", "void set_bodyA(b2Body@)", asMETHOD(b2WeldJointDefWrapper, setBodyA), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2WeldJointDef", "b2Body @get_bodyA() const", asMETHOD(b2WeldJointDefWrapper, getBodyA), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2WeldJointDef", "void set_bodyB(b2Body@)", asMETHOD(b2WeldJointDefWrapper, setBodyB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2WeldJointDef", "b2Body @get_bodyB() const", asMETHOD(b2WeldJointDefWrapper, getBodyB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "Vector2 anchor", offsetof(b2WeldJointDefWrapper, anchor)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "Vector2 localAnchorA", offsetof(b2WeldJointDefWrapper, localAnchorA)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "Vector2 localAnchorB", offsetof(b2WeldJointDefWrapper, localAnchorB)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "float referenceAngle", offsetof(b2WeldJointDefWrapper, referenceAngle)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "float frequencyHz", offsetof(b2WeldJointDefWrapper, frequencyHz)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "float dampingRatio", offsetof(b2WeldJointDefWrapper, dampingRatio)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("b2WeldJointDef", "float collideConnected", offsetof(b2WeldJointDefWrapper, collideConnected)); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("b2WeldJoint", asBEHAVE_FACTORY, "b2WeldJoint @f(const b2WeldJointDef &in)", asFUNCTION(b2WeldJointWrapper::Factory), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2WeldJoint", asBEHAVE_ADDREF, "void f()", asMETHOD(b2WeldJointWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("b2WeldJoint", asBEHAVE_RELEASE, "void f()", asMETHOD(b2WeldJointWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("b2WeldJoint", "float getReferenceAngle() const", asMETHOD(b2WeldJointWrapper, getReferenceAngle), asCALL_THISCALL); AS_ASSERT

	b2d = new Box2D;
	r = scriptEngine->RegisterGlobalProperty("ScriptBox2D Box2D", b2d);
	
#ifdef OLD
	// b2d revolute joint
	registerGlobalFunction("int  b2dCreateRevoluteJoint(int bodyA, int bodyB, float cx, float cy)", asFUNCTION(createRevoluteJoint));
	registerGlobalFunction("void b2dRemoveRevoluteJoint(int jointId)", asFUNCTION(removeRevoluteJoint));
	registerGlobalFunction("void b2dSetRevoluteJoinLimits(int jointId, float lower, float upper)", asFUNCTION(setRevoluteJoinLimits));
	registerGlobalFunction("void b2dEnableRevoluteJointLimits(int jointId)", asFUNCTION(enableRevoluteJointLimits));
	registerGlobalFunction("void b2dDisableRevoluteJointLimits(int jointId)", asFUNCTION(disableRevoluteJointLimits));
	registerGlobalFunction("void b2dSetRevoluteJointMotorSpeed(int jointId, float speed)", asFUNCTION(setRevoluteJointMotorSpeed));
	registerGlobalFunction("float b2dGetRevoluteJointMotorSpeed(int jointId)", asFUNCTION(getRevoluteJointMotorSpeed));
	registerGlobalFunction("void b2dSetRevoluteJointMaxMotorTorque(int jointId, float speed)", asFUNCTION(setRevoluteJointMaxMotorTorque));
	registerGlobalFunction("void b2dEnableRevoluteJointMotor(int jointId)", asFUNCTION(enableRevoluteJointMotor));
	registerGlobalFunction("void b2dDisableRevoluteJointMotor(int jointId)", asFUNCTION(disableRevoluteJointMotor));
	registerGlobalFunction("float b2dGetRevoluteJointAngle(int jointId)", asFUNCTION(getRevoluteJointAngle));
#endif

	return r;
}

void ReleasePlugin()
{
	delete b2d;
}

#ifdef OLD

//--------------------------------------------------------------
// Box2D Revolute Joint
//--------------------------------------------------------------

int createRevoluteJoint(int bodyA, int bodyB, float cx, float cy)
{
	// Check for valid bodies
	if(bodies.find(bodyA) == bodies.end() || bodies.find(bodyB) == bodies.end())
		return -1;

	// Create revolute joint
	b2RevoluteJointDef jd;
	jd.Initialize(bodies[bodyA], bodies[bodyB], b2Vec2(cx/worldScale, cy/worldScale));
	int id = currRevoluteJointId++;
	revoluteJoints[id] = static_cast<b2RevoluteJoint*>(world->CreateJoint(&jd));
	revoluteJoints[id]->SetUserData(getUserDataIdPtr(id));
	return id;
}

void removeRevoluteJoint(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	world->DestroyJoint(revoluteJoints[jointId]);
	revoluteJoints.erase(jointId);
}

void setRevoluteJoinLimits(int jointId, float lower, float upper)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->SetLimits(lower, upper);
}

void enableRevoluteJointLimits(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableLimit(true);
}

void disableRevoluteJointLimits(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableLimit(false);
}

void setRevoluteJointMotorSpeed(int jointId, float speed)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->SetMotorSpeed(speed);
}

float getRevoluteJointMotorSpeed(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return 0.0f;
	return revoluteJoints[jointId]->GetMotorSpeed();
}

void setRevoluteJointMaxMotorTorque(int jointId, float torque)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->SetMaxMotorTorque(torque);
}

void enableRevoluteJointMotor(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableMotor(true);
}

void disableRevoluteJointMotor(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableMotor(false);
}

float getRevoluteJointAngle(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return 0.0f;
	return revoluteJoints[jointId]->GetJointAngle();
}
#endif // OLD