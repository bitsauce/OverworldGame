#include "BlockData.h"
#include "World/World.h"

map<BlockID, BlockData*> BlockData::s_data;
TextureAtlas *BlockData::s_textureAtlas = nullptr;
Resource<Texture2D> BlockData::s_dataTexture = nullptr;

BlockData *BlockData::getByName(const string &name)
{
	for(map<BlockID, BlockData*>::iterator itr = s_data.begin(); itr != s_data.end(); ++itr)
	{
		if(itr->second && itr->second->m_name == name)
		{
			return itr->second;
		}
	}
	return 0;
}

struct BlockDataDesc
{
	const BlockID id;
	const string name;
	const string imagePath;
	const ItemID itemID;
	const float opacity;
	const uint frameCount;
};

void BlockData::init()
{
	LOG("Loading block data...");

	// Load block data from file
	vector<BlockDataDesc> blockDataDesc;
	if(util::fileExists("BlockData.xml"))
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile("BlockData.xml");

		// Get root node
		tinyxml2::XMLNode *blockNode = doc.FirstChildElement();
		if(!blockNode)
		{
			LOG("BlockData.xml has no root node!");
			return;
		}

		// For each block node
		blockNode = blockNode->FirstChildElement();
		while(blockNode)
		{
			// For each block entry
			tinyxml2::XMLElement *id = blockNode->FirstChildElement("id");
			tinyxml2::XMLElement *name = blockNode->FirstChildElement("name");
			tinyxml2::XMLElement *image = blockNode->FirstChildElement("image");
			tinyxml2::XMLElement *item = blockNode->FirstChildElement("item");
			tinyxml2::XMLElement *opacity = blockNode->FirstChildElement("opacity");
			tinyxml2::XMLElement *frames = blockNode->FirstChildElement("frames");

			if(id && name && image && item && opacity && frames)
			{
				BlockDataDesc desc = { util::strToInt(id->GetText()), name->GetText(), image->GetText(), 0, util::strToFloat(opacity->GetText()), util::strToInt(frames->GetText()) };
				blockDataDesc.push_back(desc);
			}
			else
			{
				if(name)
				{
					LOG("Loading block '%s' failed", name->GetText());
				}
				else if(id)
				{
					LOG("Loading block (id='%s') failed", id->GetText());
				}
				else
				{
					LOG("Loading block (nr='%i') failed", blockDataDesc.size());
				}
			}

			// Next resource
			blockNode = blockNode->NextSibling();
		}
	}
	else
	{
		THROW("BlockData.xml is missing!");
	}

	// Block data pixmap
	Pixmap blockDataPixmap(blockDataDesc.size(), 2);
	uchar pixelData[4];

	vector<Pixmap> pixmaps(blockDataDesc.size());
	for(int i = 0; i < blockDataDesc.size(); i++)
	{
		BlockDataDesc *blockData = &blockDataDesc[i];

		// Create block data object
		Pixmap pixmap(blockData->imagePath, true);
		s_data[blockData->id] = new BlockData(blockData->id, blockData->name, pixmap, blockData->itemID, blockData->opacity);
		pixmaps[blockData->id] = pixmap;

		// Store meta data
		pixelData[0] = blockData->frameCount;
		pixelData[1] = 0;
		pixelData[2] = 0;
		pixelData[3] = 0;
		blockDataPixmap.setPixel(blockData->id, 1, pixelData);

		// Next block desc
		blockData++;
	}

	// Create block atlas
	s_textureAtlas = new TextureAtlas(pixmaps, 0);

	// Fill block UV data
	for(int id = 0; id < blockDataDesc.size(); ++id)
	{
		Vector2I pos = s_textureAtlas->get(id).uv0 * s_textureAtlas->getTexture()->getSize();

		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);

		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);

		blockDataPixmap.setPixel(id, 0, pixelData);
	}
	s_dataTexture = Resource<Texture2D>(new Texture2D(blockDataPixmap));
	s_dataTexture->setFiltering(Texture2D::NEAREST);
}

BlockData::BlockData(const BlockID id, const string &name, const Pixmap &pixmap, const ItemID item, const float opacity) :
	m_name(name),
	m_id(id),
	m_pixmap(pixmap),
	m_item(item),
	m_opacity(opacity)
{
}

BlockData *BlockData::get(const BlockID block)
{
	return s_data[block];
}