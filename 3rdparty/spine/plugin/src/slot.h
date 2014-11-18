#ifndef PLUGIN_SLOT_H
#define PLUGIN_SLOT_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class spAttachmentWrapper;

class spSlotWrapper
{
public:
	spSlotWrapper(spSlot *anim);
	spSlot *get() { return m_self; }
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

private:
	RefCounter refCounter;
	spSlot *m_self;
};

#endif // PLUGIN_SLOT_H