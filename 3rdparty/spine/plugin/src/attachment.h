#ifndef PLUGIN_ATTACHMENT_H
#define PLUGIN_ATTACHMENT_H

#include <x2d/x2d.h>
#include <spine/spine.h>

class spSlotWrapper;

class spRegionAttachmentWrapper
{
public:
	spRegionAttachmentWrapper(const string &name, spSlotWrapper *slot);
	~spRegionAttachmentWrapper();
	spRegionAttachment *get() { return m_self; }
	
	void setPosition(const Vector2 &position);
	void setRotation(const float rotation);
	void setScale(const Vector2 &scale);
	void setSize(const Vector2 &size);
	void setTextureRegion(const XTextureRegion &region);
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }
	
	static spRegionAttachmentWrapper *Factory(const string &name, spSlotWrapper *slot)
	{
		return new spRegionAttachmentWrapper(name, slot);
	}

private:
	RefCounter refCounter;
	spRegionAttachment *m_self;
};

#endif // PLUGIN_ATTACHMENT_H