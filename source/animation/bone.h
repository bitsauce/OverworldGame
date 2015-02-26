#ifndef BONE_H
#define BONE_H

#include "Config.h"
#include <spine/spine.h>

class Bone
{
public:
	Bone(spBone *bone);

	void setPosition(const Vector2 &position);
	void setRotation(const float rotation);
	void setScale(const Vector2 &scale);
	Vector2 getWorldPosition() const;
	float getWorldRotation() const;
	Vector2 getWorldScale() const;

private:
	spBone *m_self;
};

#endif // BONE_H