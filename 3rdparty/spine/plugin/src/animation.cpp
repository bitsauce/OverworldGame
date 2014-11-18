#include "plugin.h"
#include "animation.h"
#include "skeleton.h"
#include "event.h"

#include <spine/spine.h>

int spAnimationStateWrapper::TypeId = 0;

spAnimationWrapper::spAnimationWrapper(spSkeleton *skeleton, spAnimation *anim) :
	m_self(anim),
	m_skeleton(skeleton),
	m_lastTime(0.0f),
	m_looping(false)
{
}

spAnimation *spAnimationWrapper::get()
{
	return m_self;
}

void spAnimationWrapper::setTime(const float time)
{
	m_lastTime = time;
}

float spAnimationWrapper::getTime() const
{
	return m_lastTime;
}

void spAnimationWrapper::setLooping(const bool looping)
{
	m_looping = looping;
}

bool spAnimationWrapper::getLooping() const
{
	return m_looping;
}

void spAnimationWrapper::apply(const float dt)
{
	float time = m_lastTime + dt;
	spAnimation_apply(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0);
	m_lastTime = time;
}

void spAnimationWrapper::mix(const float dt, const float alpha)
{
	float time = m_lastTime + dt;
	spAnimation_mix(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0, alpha);
	m_lastTime = time;
}

spAnimationStateDataWrapper::spAnimationStateDataWrapper(spSkeletonWrapper *skeleton) :
	m_skeleton(skeleton)
{
	m_self = spAnimationStateData_create(m_skeleton->data());
}

spAnimationStateDataWrapper::~spAnimationStateDataWrapper()
{
	m_skeleton->release();
	spAnimationStateData_dispose(m_self);
}

void spAnimationStateDataWrapper::setMix(const string &fromName, const string &toName, const float duration)
{
	spAnimationStateData_setMixByName(m_self, fromName.c_str(), toName.c_str(), duration);
}

void spAnimationStateDataWrapper::setMix(spAnimationWrapper *from, spAnimationWrapper *to, const float duration)
{
	spAnimationStateData_setMix(m_self, from->get(), to->get(), duration);
}

float spAnimationStateDataWrapper::getMix(spAnimationWrapper *from, spAnimationWrapper *to) const
{
	return spAnimationStateData_getMix(m_self, from->get(), to->get());
}

spSkeletonWrapper *spAnimationStateDataWrapper::getSkeleton() const
{
	return m_skeleton;
}

spAnimationStateData *spAnimationStateDataWrapper::get() const
{
	return m_self;
}

spAnimationStateDataWrapper *spAnimationStateDataWrapper::Factory(spSkeletonWrapper *skeleton)
{
	return skeleton != 0 ? new spAnimationStateDataWrapper(skeleton) : 0;
}

void eventListener(spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount)
{
	spEventWrapper *wrappedEvent = new spEventWrapper(event, type, loopCount);
	wrappedEvent->call(((spAnimationStateWrapper*)state->rendererObject)->getEventCallback());
	wrappedEvent->release();
}

spAnimationStateWrapper::spAnimationStateWrapper(spAnimationStateDataWrapper *data) :
	m_data(data),
	m_looping(false),
	m_eventCallback(0)
{
	m_self = spAnimationState_create(data->get());
	m_self->listener = &eventListener;
	m_self->rendererObject = this;
}

spAnimationStateWrapper::~spAnimationStateWrapper()
{
	spAnimationState_dispose(m_self);
}

void spAnimationStateWrapper::setLooping(const bool looping)
{
	m_looping = looping;
}

bool spAnimationStateWrapper::getLooping() const
{
	return m_looping;
}

void spAnimationStateWrapper::setTimeScale(const float timeScale)
{
	m_self->timeScale = timeScale;
}

float spAnimationStateWrapper::getTimeScale() const
{
	return m_self->timeScale;
}

void spAnimationStateWrapper::setEventCallback(asIScriptFunction *func)
{
	if(m_eventCallback) {
		m_eventCallback->Release();
	}
	m_eventCallback = func;
}

asIScriptFunction *spAnimationStateWrapper::getEventCallback() const
{
	if(m_eventCallback) {
		m_eventCallback->AddRef();
	}
	return m_eventCallback;
}

void spAnimationStateWrapper::setAnimation(const string &name)
{
	spAnimationState_setAnimationByName(m_self, 0, name.c_str(), m_looping);
}

void spAnimationStateWrapper::setAnimation(spAnimationWrapper *anim)
{
	spAnimationState_setAnimation(m_self, 0, anim->get(), m_looping);
}

void spAnimationStateWrapper::update(const float delta)
{
	spAnimationState_update(m_self, delta);
	spAnimationState_apply(m_self, m_data->getSkeleton()->get());
}

void spAnimationStateWrapper::addAnimation(spAnimationWrapper *anim, const float delay)
{
	spAnimationState_addAnimation(m_self, 0, anim->get(), m_looping, delay);
}

void spAnimationStateWrapper::addAnimation(const string &name, const float delay)
{
	spAnimationState_addAnimationByName(m_self, 0, name.c_str(), m_looping, delay);
}

void spAnimationStateWrapper::enumReferences(asIScriptEngine *engine)
{
	engine->GCEnumCallback(m_data);
	if(m_eventCallback) {
		engine->GCEnumCallback(m_eventCallback);
	}
}

void spAnimationStateWrapper::releaseReferences(asIScriptEngine *engine)
{
	m_data->release();
	if(m_eventCallback) {
		m_eventCallback->Release();
	}
}

spAnimationStateWrapper *spAnimationStateWrapper::Factory(spAnimationStateDataWrapper *data)
{
	if(!data) return 0;

	spAnimationStateWrapper *animState = new spAnimationStateWrapper(data);

	XScriptEngine::GetAngelScript()->NotifyGarbageCollectorOfNewObject(animState, XScriptEngine::GetAngelScript()->GetObjectTypeById(spAnimationStateWrapper::TypeId));

	return animState;
}