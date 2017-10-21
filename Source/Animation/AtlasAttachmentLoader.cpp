#include "AtlasAttachmentLoader.h"
#include "RegionAttachment.h"
#include <spine/extension.h>

AtlasAttachmentLoader::AtlasAttachmentLoader(const string &atlasFile)
{
	m_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	m_self = spAtlasAttachmentLoader_create(m_atlas);
}

RegionAttachment *AtlasAttachmentLoader::newAttachment(const string &name, const string &path)
{
	spAttachment *attachment = spAttachmentLoader_newAttachment(SUPER(m_self), 0, SP_ATTACHMENT_REGION, name.c_str(), path.c_str());
	spRegionAttachment *regionAttachment = SUB_CAST(spRegionAttachment, attachment);
	spAtlasRegion *atlasRegion = spAtlas_findRegion(m_atlas, path.c_str());

	if(atlasRegion)
	{
		regionAttachment->width = atlasRegion->width;
		regionAttachment->height = atlasRegion->height;
		return new RegionAttachment(regionAttachment);
	}

	return 0;
}
