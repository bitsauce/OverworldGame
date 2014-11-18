#ifndef PLUGIN_ANIMATION_H
#define PLUGIN_ANIMATION_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class spSkeletonWrapper;
class spEventWrapper;

class spAnimationWrapper
{
public:
	spAnimationWrapper(spSkeleton *skeleton, spAnimation *anim);

	spAnimation *get();
	void setTime(const float time);
	float getTime() const;
	void setLooping(const bool looping);
	bool getLooping() const;
	void apply(const float dt);
	void mix(const float dt, const float alpha);
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

private:
	RefCounter refCounter;
	spAnimation *m_self;
	spSkeleton *m_skeleton;
	float m_lastTime;
	bool m_looping;
};

class spAnimationStateDataWrapper
{
public:
	spAnimationStateDataWrapper(spSkeletonWrapper *data);
	~spAnimationStateDataWrapper();

	void setMix(const string &fromName, const string &toName, const float duration);
	void setMix(spAnimationWrapper *from, spAnimationWrapper *to, const float duration);
	float getMix(spAnimationWrapper *from, spAnimationWrapper *to) const;
	spSkeletonWrapper *getSkeleton() const;
	spAnimationStateData *get() const;

	static spAnimationStateDataWrapper *Factory(spSkeletonWrapper *skeleton);
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

private:
	RefCounter refCounter;
	spAnimationStateData *m_self;
	spSkeletonWrapper *m_skeleton;
};

class spAnimationStateWrapper
{
	friend class spEventWrapper;
public:
	spAnimationStateWrapper(spAnimationStateDataWrapper *data);
	~spAnimationStateWrapper();
	
	void setLooping(const bool looping);
	bool getLooping() const;

	void setTimeScale(const float timeScale);
	float getTimeScale() const;

	void setEventCallback(asIScriptFunction *func);
	asIScriptFunction *getEventCallback() const;

	void setAnimation(const string &name);
	void setAnimation(spAnimationWrapper *anim);
	void addAnimation(const string &name, const float delay);
	void addAnimation(spAnimationWrapper *anim, const float delay);
	void update(const float delta);

	static spAnimationStateWrapper *Factory(spAnimationStateDataWrapper *data);

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

	static int TypeId;

private:
	RefCounter refCounter;
	bool gcFlag;
	spAnimationStateDataWrapper *m_data;
	spAnimationState *m_self;
	bool m_looping;
	asIScriptFunction *m_eventCallback;
};

#endif // PLUGIN_ANIMATION_H