#include "Atlas.h"

SpineAtlas::SpineAtlas(const string &filePath)
{
	m_self = spAtlas_createFromFile(filePath.c_str(), 0);

	spAtlasRegion *region = m_self->regions;
	while(region)
	{
		m_regions[region->name] = new SpineAtlasRegion(region);
		region = region->next;
	}
}


SpineAtlas::~SpineAtlas()
{
	for(map<string, SpineAtlasRegion*>::iterator itr = m_regions.begin(); itr != m_regions.end(); ++itr)
	{
		delete itr->second;
	}

	spAtlas_dispose(m_self);
}

SpineAtlasRegion *SpineAtlas::findRegion(const string &name)
{
	return m_regions.find(name) != m_regions.end() ? m_regions[name] : nullptr;
}

Resource<Texture2D> SpineAtlas::getTexture() const
{
	return *(Resource<Texture2D>*)m_self->pages->rendererObject;
}

SpineAtlasRegion::SpineAtlasRegion() :
	m_self(spAtlasRegion_create())
{
}

SpineAtlasRegion::SpineAtlasRegion(spAtlasRegion *atlasRegion) :
	m_self(atlasRegion)
{
}

TextureRegion SpineAtlasRegion::getTextureRegion() const
{
	return TextureRegion(m_self->u, m_self->v, m_self->u2, m_self->v2);
}
