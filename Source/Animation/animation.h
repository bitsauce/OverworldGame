#ifndef ANIMATION_H
#define ANIMATION_H

#include "Config.h"
#include <spine/spine.h>

class Skeleton;

class Animation
{
	friend class AnimationState;
	friend class AnimationStateData;
public:
	Animation(spSkeleton *skeleton, spAnimation *anim);
	void setTime(const float time);
	void setTimeAndMix(const float time, const float alpha);
	float getTime() const;
	void setLooping(const bool looping);
	bool getLooping() const;
	void apply(const float delta);
	void mix(const float delta, const float alpha);

private:
	spAnimation *m_self;
	spSkeleton *m_skeleton;
	float m_lastTime;
	bool m_looping;
};

class AnimationStateData
{
	friend class AnimationState;
public:
	AnimationStateData(Skeleton *skeleton);
	~AnimationStateData();
	void setMix(const string &fromName, const string &toName, const float duration);
	void setMix(Animation *from, Animation *to, const float duration);
	float getMix(Animation *from, Animation *to) const;

private:
	spAnimationStateData *m_self;
	Skeleton *m_skeleton;
};

class AnimationState
{
	friend class spEventWrapper;
public:
	AnimationState(AnimationStateData *data);
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

	void setTime(const float time);
	float getTime() const;

	void onTick(TickEvent *e);

private:
	AnimationStateData *m_data;
	spAnimationState *m_self;
	bool m_looping;
	float m_prevTime;
	//asIScriptFunction *m_eventCallback;
};

#endif // ANIMATION_H