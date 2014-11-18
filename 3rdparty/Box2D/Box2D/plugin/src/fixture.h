#ifndef B2_FIXTURE_WRAPPER_H
#define B2_FIXTURE_WRAPPER_H

#include <x2d/x2d.h>

class b2Fixture;
class b2Shape;

class b2FixtureWrapper
{
	friend class b2BodyWrapper;
public:
	b2FixtureWrapper(b2BodyWrapper *body, b2Fixture *fixture);
	~b2FixtureWrapper();
	void destroy();
	
	RefCounter refCounter;
	bool gcFlag;
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
	
	void setFriction(const float friction);
	void setDensity(const float density);
	void setRestitution(const float restitution);
	void setSensor(const bool sensor);
	bool isSensor() const;
	void setMaskBits(const uint maskBits);
	void setCategoryBits(const uint categoryBits);
	
	b2BodyWrapper *getBody() const;

	static int TypeId;

private:
	b2Fixture *m_fixture;
	b2BodyWrapper *m_body;
};

#endif // B2_FIXTURE_WRAPPER_H