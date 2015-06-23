

#include <spine/spine.h>
#include "Animation.h"
#include "Skeleton.h"
//#include "event.h"

Animation::Animation(spSkeleton *skeleton, spAnimation *anim) :
	m_self(anim),
	m_skeleton(skeleton),
	m_lastTime(0.0f),
	m_looping(false)
{
}

void Animation::setTime(const float time)
{
	spAnimation_apply(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0);
	m_lastTime = time;
}

float Animation::getTime() const
{
	return m_lastTime;
}

void Animation::setLooping(const bool looping)
{
	m_looping = looping;
}

bool Animation::getLooping() const
{
	return m_looping;
}

void Animation::apply(const float delta)
{
	float time = m_lastTime + delta;
	spAnimation_apply(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0);
	m_lastTime = time;
}

void Animation::mix(const float delta, const float alpha)
{
	float time = m_lastTime + delta;
	spAnimation_mix(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0, alpha);
	m_lastTime = time;
}

AnimationStateData::AnimationStateData(Skeleton *skeleton) :
	m_skeleton(skeleton)
{
	m_self = spAnimationStateData_create(m_skeleton->m_data);
}

AnimationStateData::~AnimationStateData()
{
	spAnimationStateData_dispose(m_self);
}

void AnimationStateData::setMix(const string &fromName, const string &toName, const float duration)
{
	spAnimationStateData_setMixByName(m_self, fromName.c_str(), toName.c_str(), duration);
}

void AnimationStateData::setMix(Animation *from, Animation *to, const float duration)
{
	spAnimationStateData_setMix(m_self, from->m_self, to->m_self, duration);
}

float AnimationStateData::getMix(Animation *from, Animation *to) const
{
	return spAnimationStateData_getMix(m_self, from->m_self, to->m_self);
}

/*void eventListener(spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount)
{
	spEventWrapper *wrappedEvent = new spEventWrapper(event, type, loopCount);
	wrappedEvent->call(((spAnimationStateWrapper*)state->rendererObject)->getEventCallback());
	wrappedEvent->release();
}*/

AnimationState::AnimationState(AnimationStateData *data) :
	m_data(data),
	m_looping(false),
	m_prevTime(0.0f)/*,
	m_eventCallback(0)*/
{
	m_self = spAnimationState_create(data->m_self);
	//m_self->listener = &eventListener;
	m_self->rendererObject = this;
}

AnimationState::~AnimationState()
{
	spAnimationState_dispose(m_self);
}

void AnimationState::setLooping(const bool looping)
{
	m_looping = looping;
}

bool AnimationState::getLooping() const
{
	return m_looping;
}

void AnimationState::setTimeScale(const float timeScale)
{
	m_self->timeScale = timeScale;
}

float AnimationState::getTimeScale() const
{
	return m_self->timeScale;
}

/*void AnimationState::setEventCallback(asIScriptFunction *func)
{
	if(m_eventCallback) {
		m_eventCallback->Release();
	}
	m_eventCallback = func;
}

asIScriptFunction *AnimationState::getEventCallback() const
{
	if(m_eventCallback) {
		m_eventCallback->AddRef();
	}
	return m_eventCallback;
}*/

void AnimationState::setAnimation(const string &name)
{
	spAnimationState_setAnimationByName(m_self, 0, name.c_str(), m_looping);
}

void AnimationState::setAnimation(Animation *anim)
{
	spAnimationState_setAnimation(m_self, 0, anim->m_self, m_looping);
}

void AnimationState::update(const float delta)
{
	m_prevTime += delta;
	spAnimationState_update(m_self, delta);
	spAnimationState_apply(m_self, m_data->m_skeleton->m_self);
}

void AnimationState::addAnimation(Animation *anim, const float delay)
{
	spAnimationState_addAnimation(m_self, 0, anim->m_self, m_looping, delay);
}

void AnimationState::addAnimation(const string &name, const float delay)
{
	spAnimationState_addAnimationByName(m_self, 0, name.c_str(), m_looping, delay);
}

void AnimationState::setTime(const float time)
{
	spAnimationState_update(m_self, time - m_prevTime);
	spAnimationState_apply(m_self, m_data->m_skeleton->m_self);
	m_prevTime = time;
}

float AnimationState::getTime() const
{
	return m_prevTime;
}