#include "Attachment.h"
#include <spine/extension.h>

Attachment::Attachment()
{
	m_self = spRegionAttachment_create("test");

	Resource<Texture2D> *texture = new Resource<Texture2D>(new Texture2D(
		Pixmap("Sprites/Items/Box.png")));

	spAtlasRegion *a=spAtlasRegion_create();
	a->page = spAtlasPage_create(0, "");
	a->page->rendererObject = texture;

	m_self->rendererObject = a;
	
	// x, y = origin
	m_self->x = m_self->y = 16;

	spRegionAttachment_setUVs(m_self, 0, 0, 1, 1, 0);
	m_self->regionOffsetX = 0;
	m_self->regionOffsetY = 0;
	m_self->regionWidth = 16;
	m_self->regionHeight = 16;
	m_self->regionOriginalWidth = 16;
	m_self->regionOriginalHeight = 16;
	m_self->width = m_self->height = 16;
	spRegionAttachment_updateOffset(m_self);
}