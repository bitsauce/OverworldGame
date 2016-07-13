#ifndef ANIM_ATTACHMENT_H
#define ANIM_ATTACHMENT_H

#include "Config.h"
#include <spine/spine.h>

class Attachment
{
	friend class Slot;
public:
	Attachment();

private:
	spRegionAttachment *m_self;
};

#endif // ANIM_ATTACHMENT_H