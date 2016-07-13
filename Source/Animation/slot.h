#ifndef SLOT_H
#define SLOT_H

#include "Config.h"
#include <spine/spine.h>

class Attachment;

class Slot
{
public:
	Slot(spSlot *slot);

	void setAttachment(Attachment *attachment);

private:
	spSlot *m_self;
};

#endif // SLOT_H