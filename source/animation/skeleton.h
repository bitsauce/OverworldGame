#ifndef SKELETON_H
#define SKELETON_H

#include <spine/spine.h>
#include <x2d/x2d.h>

class Animation;
class Slot;
class Bone;

class Skeleton
{
public:
	Skeleton(const string &jsonFile, const string &atlasFile, const float scale);
	~Skeleton();

	Animation *findAnimation(const string &name);
	Slot *findSlot(const string &name);
	Bone *findBone(const string &name);
	void setPosition(const Vector2 &pos);
	Vector2 getPosition() const;
	void setFlipX(const bool flip);
	bool getFlipX() const;
	void setFlipY(const bool flip);
	bool getFlipY() const;
	shared_ptr<XTexture> getTexture() const;

	void draw(XBatch *batch);

private:
	spSkeleton *m_self;
	spSkeletonData *m_data;
	spAtlas* m_atlas;
	float *m_worldVertices;
	map<string, Animation*> m_animations;
	map<string, Slot*> m_slots;
	map<string, Bone*> m_bones;
};

#endif // SKELETON_H