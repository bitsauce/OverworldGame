#include "slot.h"
#include "Attachment.h"
#include <spine/extension.h>

Slot::Slot(spSlot *slot) :
	m_self(slot)
{
}

void Slot::setAttachment(Attachment *attachment)
{
	if(attachment)
	{
		spSlot_setAttachment(m_self, SUPER(attachment->m_self));
	}
	else
	{
		spSlot_setAttachment(m_self, 0);
	}
}