#include "attachment.h"
#include "slot.h"
#include "spine/extension.h"

spRegionAttachmentWrapper::spRegionAttachmentWrapper(const string &name, spSlotWrapper *slot)
{
	m_self = spRegionAttachment_create(name.c_str());
	spSlot_setAttachment(slot->get(), SUPER(m_self));
	slot->release();
}

spRegionAttachmentWrapper::~spRegionAttachmentWrapper()
{
}

void spRegionAttachmentWrapper::setPosition(const Vector2 &position)
{
	m_self->x = position.x;
	m_self->y = position.y;
}

void spRegionAttachmentWrapper::setRotation(const float rotation)
{
	m_self->rotation = rotation;
}

void spRegionAttachmentWrapper::setScale(const Vector2 &scale)
{
	m_self->scaleX = scale.x;
	m_self->scaleY = scale.y;
}

void spRegionAttachmentWrapper::setSize(const Vector2 &size)
{
	m_self->width = size.x;
	m_self->height = size.y;
}

void spRegionAttachmentWrapper::setTextureRegion(const XTextureRegion &region)
{
	spRegionAttachment_setUVs(m_self, region.uv0.x, region.uv0.y, region.uv1.x, region.uv1.y, false);
}