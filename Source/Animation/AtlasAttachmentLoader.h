#include "Config.h"
#include <spine/spine.h>

class RegionAttachment;

class AtlasAttachmentLoader
{
public:
	AtlasAttachmentLoader(const string &atlasFile);

	RegionAttachment *newAttachment(const string &name, const string &path);

private:
	spAtlas *m_atlas;
	spAtlasAttachmentLoader *m_self;
};