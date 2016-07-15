#ifndef ANIM_ATTACHMENT_H
#define ANIM_ATTACHMENT_H

#include "Config.h"
#include <spine/spine.h>

class RegionAttachment
{
	friend class Slot;
public:
	RegionAttachment(spRegionAttachment *attachment);

	void setPosition(const float x, const float y);
	void setSize(const float w, const float h);
	void setRotation(const float angle);

private:
	spRegionAttachment *m_self;
};

#endif // ANIM_ATTACHMENT_H