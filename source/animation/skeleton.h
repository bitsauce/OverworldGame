#ifndef SKELETON_H
#define SKELETON_H
//>REMOVE ME<
#include "Config.h"
#include <spine/spine.h>

class Animation;
class Slot;
class Bone;

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
	void setPosition(const Vector2 &pos);
	Vector2 getPosition() const;
	void setFlipX(const bool flip);
	bool getFlipX() const;
	void setFlipY(const bool flip);
	bool getFlipY() const;
	Texture2DPtr getTexture() const;
	TextureRegion getTextureRegion(const string &name) const;

	void draw(GraphicsContext &gfxContext);

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