#include "RegionAttachment.h"
#include <spine/extension.h>

RegionAttachment::RegionAttachment(spRegionAttachment *attachment) :
	m_self(attachment)
{
	spRegionAttachment_updateOffset(m_self);
}

void RegionAttachment::setPosition(const float x, const float y)
{
	m_self->x = x;
	m_self->y = y;
	spRegionAttachment_updateOffset(m_self);
}

void RegionAttachment::setSize(const float w, const float h)
{
	m_self->width = w;
	m_self->height = h;
	spRegionAttachment_updateOffset(m_self);
}

void RegionAttachment::setRotation(const float angle)
{
	m_self->rotation = angle;
	spRegionAttachment_updateOffset(m_self);
}
