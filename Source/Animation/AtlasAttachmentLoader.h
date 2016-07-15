#include "Config.h"
#include <spine/spine.h>

class RegionAttachment;

class AtlasAttachmentLoader
{
public:
	AtlasAttachmentLoader(const string &atlasFile);

	RegionAttachment *getAttachment(const string &name);

private:
	spAtlas *m_atlas;
	spAtlasAttachmentLoader *m_self;
};