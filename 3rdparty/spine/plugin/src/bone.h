#ifndef PLUGIN_BONE_H
#define PLUGIN_BONE_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class spBoneWrapper
{
public:
	spBoneWrapper(spBone *anim);
	spBone *get() { return m_self; }

	void setPosition(const Vector2 &position);
	void setRotation(const float rotation);
	void setScale(const Vector2 &scale);
	Vector2 getWorldPosition() const;
	float getWorldRotation() const;
	Vector2 getWorldScale() const;
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

private:
	RefCounter refCounter;
	spBone *m_self;
};

#endif // PLUGIN_BONE_H