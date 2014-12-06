#ifndef SLOT_H
#define SLOT_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class Slot
{
public:
	Slot(spSlot *slot);

private:
	spSlot *m_self;
};

#endif // SLOT_H