#ifndef B2_CONTACT_WRAPPER_H
#define B2_CONTACT_WRAPPER_H

#include <x2d/x2d.h>

class b2Contact;
class b2BodyWrapper;
class b2FixtureWrapper;

class b2ContactWrapper
{
public:
	b2ContactWrapper(b2Contact *contact, b2FixtureWrapper *fixtureA, b2FixtureWrapper *fixtureB);
	~b2ContactWrapper();

	enum ContactType
	{
		BeginContact,
		EndContact,
		PreSolve,
		PostSolve
	};

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	void setEnabled(bool enabled);
	bool isTouching() const;
	
	void setFriction(float friction);
	float getFriction() const;
	void resetFriction();

	void setRestitution(float restitution);
	float getRestitution() const;
	void resetRestitution();
	
	void swapAB();
	void call(ContactType stage);
	bool isValid() const;

	b2BodyWrapper *getBodyA() const;
	b2BodyWrapper *getBodyB() const;
	b2FixtureWrapper *getFixtureA() const;
	b2FixtureWrapper *getFixtureB() const;

private:
	b2Contact *m_contact;
	b2FixtureWrapper *m_fixtureA;
	b2FixtureWrapper *m_fixtureB;
};

#endif // B2_CONTACT_WRAPPER_H