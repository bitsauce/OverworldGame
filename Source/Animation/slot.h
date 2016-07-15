#ifndef SLOT_H
#define SLOT_H

#include "Config.h"
#include <spine/spine.h>

class RegionAttachment;

class Slot
{
public:
	Slot(spSlot *slot);

	void setAttachment(RegionAttachment *attachment);

private:
	spSlot *m_self;
};

#endif // SLOT_H