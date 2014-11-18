#ifndef B2_CONTACTLISTENER_H
#define B2_CONTACTLISTENER_H

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2ContactManager.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>

class xdScriptEngine;

class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif // B2_CONTACTLISTENER_H