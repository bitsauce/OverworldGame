#include "event.h"
#include "animation.h"

int spEventWrapper::TypeId = 0;

spEventWrapper::spEventWrapper(spEvent *event, spEventType type, int loopCount) :
	m_self(event),
	m_type(type),
	m_loopCount(loopCount)
{
}

string spEventWrapper::getString() const
{
	return m_self != 0 ? m_self->data->stringValue : "";
}

int spEventWrapper::getInt() const
{
	return m_self != 0 ? m_self->data->intValue : 0;
}

float spEventWrapper::getFloat() const
{
	return m_self != 0 ? m_self->data->floatValue : 0.0f;
}

spEventType spEventWrapper::getType() const
{
	return m_type;
}

int spEventWrapper::getLoopCount() const
{
	return m_loopCount;
}

void spEventWrapper::call(asIScriptFunction *func)
{
	if(func)
	{
		XFuncCall funcCall(func);
		spEventWrapper *self = this;
		addRef();
		funcCall.setArg(0, &self, spEventWrapper::TypeId | asTYPEID_OBJHANDLE);
		funcCall.execute();
		func->Release();
	}
}