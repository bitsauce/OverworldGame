#ifndef ANIMATION_H
#define ANIMATION_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class Skeleton;
class Event;

class Animation
{
public:
	Animation(spSkeleton *skeleton, spAnimation *anim);
	void setTime(const float time);
	float getTime() const;
	void setLooping(const bool looping);
	bool getLooping() const;
	void apply(const float dt);
	void mix(const float dt, const float alpha);

private:
	spAnimation *m_self;
	spSkeleton *m_skeleton;
	float m_lastTime;
	bool m_looping;
};
class AnimationStateData
{
public:
	AnimationStateData(Skeleton *data);
	~AnimationStateData();
	void setMix(const string &fromName, const string &toName, const float duration);
	void setMix(Animation *from, Animation *to, const float duration);
	float getMix(Animation *from, Animation *to) const;
	spSkeletonWrapper *getSkeleton() const;

private:
	spAnimationStateData *m_self;
	spSkeletonWrapper *m_skeleton;
};
class AnimationState
{
friend class spEventWrapper;
public:
	AnimationState(const AnimationStateData &data);
	~AnimationState();

	void setLooping(const bool looping);
	bool getLooping() const;

	void setTimeScale(const float timeScale);
	float getTimeScale() const;

	//void setEventCallback(asIScriptFunction *func);
	//asIScriptFunction *getEventCallback() const;

	void setAnimation(const string &name);
	void setAnimation(Animation *anim);
	void addAnimation(const string &name, const float delay);
	void addAnimation(Animation *anim, const float delay);

	void update(const float delta);

private:
	AnimationStateData *m_data;
	spAnimationState *m_self;
	bool m_looping;
	//asIScriptFunction *m_eventCallback;
};

#endif // ANIMATION_H