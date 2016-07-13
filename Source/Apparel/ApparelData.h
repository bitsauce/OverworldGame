#ifndef APPAREL_DATA_H
#define APPAREL_DATA_H
/*
#include "Config.h"

enum ApparelID;

class ApparelData
{
public:
	// Constructors
	ApparelData(const ApparelID id);

	// Data getters
	ApparelID getID() const
	{
		return m_id;
	}

	//ItemID getItem() const
	//{
	//	return m_item;
	//}
	
	Pixmap getPixmap() const
	{
		return m_pixmap;
	}
	
	float getOpacity() const
	{
		return m_opacity;
	}

	// Static functions
	static TextureAtlas *getTextureAtlas()
	{
		return s_textureAtlas;
	}

	static Resource<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}

	static void init();
	static ApparelData *get(const ApparelID id);

private:
	const ApparelID m_id;
	//const ItemID m_item;
	const Pixmap m_pixmap;
	const float m_opacity;

	// Static data
	static vector<ApparelData*> s_data;
	static TextureAtlas *s_textureAtlas;
	static Resource<Texture2D> s_dataTexture;
};
*/
#endif // APPAREL_DATA_H