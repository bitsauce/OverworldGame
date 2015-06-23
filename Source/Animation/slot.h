#ifndef SLOT_H
#define SLOT_H

#include "Config.h"
#include <spine/spine.h>

class Slot
{
public:
	Slot(spSlot *slot);

private:
	spSlot *m_self;
};

#endif // SLOT_H