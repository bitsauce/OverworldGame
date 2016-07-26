#pragma once

#include "Config.h"
#include <spine/spine.h>

class SpineAtlasRegion
{
	friend class SpineAtlas;
private:
	SpineAtlasRegion(spAtlasRegion *atlasRegion);

public:
	SpineAtlasRegion();
	TextureRegion getTextureRegion() const;

private:
	spAtlasRegion *m_self;
};

class SpineAtlas
{
	friend class Skeleton;
public:
	SpineAtlas(const string &filePath);
	~SpineAtlas();

	SpineAtlasRegion *findRegion(const string &name);

	Resource<Texture2D> getTexture() const;

private:
	spAtlas *m_self;
	map<string, SpineAtlasRegion*> m_regions;
};