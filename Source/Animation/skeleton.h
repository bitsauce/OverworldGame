#ifndef SKELETON_H
#define SKELETON_H

#include "Config.h"
#include <spine/spine.h>

class Animation;
class Slot;
class Bone;
class SpineAtlas;

class Skeleton
{
	friend class AnimationStateData;
	friend class AnimationState;
public:
	Skeleton(const string &jsonFile, const string &atlasFile, const float scale);
	~Skeleton();

	Animation *findAnimation(const string &name);
	Slot *findSlot(const string &name);
	Bone *findBone(const string &name);
	void setPosition(const Vector2F &pos);
	Vector2F getPosition() const;
	void setFlipX(const bool flip);
	bool getFlipX() const;
	void setFlipY(const bool flip);
	bool getFlipY() const;
	SpineAtlas *getAtlas() const;

	void draw(GraphicsContext *gfxContext);

private:
	spSkeleton *m_self;
	spSkeletonData *m_data;
	float *m_worldVertices;

	SpineAtlas* m_atlas;
	map<string, Animation*> m_animations;
	map<string, Slot*> m_slots;
	map<string, Bone*> m_bones;
};

#endif // SKELETON_H