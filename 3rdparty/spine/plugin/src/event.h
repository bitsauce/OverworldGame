#ifndef PLUGIN_EVENT_H
#define PLUGIN_EVENT_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class spEventWrapper
{
public:
	spEventWrapper(spEvent *e, spEventType type, int loopCount);

	string getString() const;
	int getInt() const;
	float getFloat() const;
	spEventType getType() const;
	int getLoopCount() const;

	void call(asIScriptFunction *func);
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	static int TypeId;

private:
	RefCounter refCounter;
	spEvent *m_self;
	spEventType m_type;
	int m_loopCount;
};

#endif // PLUGIN_EVENT_H