#include "BlockData.h"
#include "Items/ItemData.h"
#include "World/World.h"

map<BlockID, BlockData*> BlockData::s_idToData;
map<string, BlockData*> BlockData::s_nameToData;
TextureAtlas *BlockData::s_textureAtlas = nullptr;
shared_ptr<Texture2D> BlockData::s_dataTexture = nullptr;

BlockData *BlockData::get(const BlockID id)
{
	map<BlockID, BlockData*>::iterator itr;
	if((itr = s_idToData.find(id)) != s_idToData.end())
	{
		return itr->second;
	}
	return 0;
}

BlockData *BlockData::get(const string &name)
{
	map<string, BlockData*>::iterator itr;
	if((itr = s_nameToData.find(name)) != s_nameToData.end())
	{
		return itr->second;
	}
	return 0;
}

struct BlockDataDesc
{
	const BlockID id;
	const string name;
	const map<uint, string> images;
	const string itemName;
	const float opacity;
	const uint frameCount;
	const bool solid; // TODO: Collision mask maybe
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
			tinyxml2::XMLElement *solid = blockNode->FirstChildElement("solid");

			map<uint, string> images;
			if(!image)
			{
				tinyxml2::XMLElement *image = blockNode->FirstChildElement("images");
				if(image)
				{
					image = image->FirstChildElement();
					while(image)
					{
						uint id = util::strToInt(image->Attribute("id"));
						images[id] = image->GetText();
						image = image->NextSiblingElement();
					}
				}
			}
			else
			{
				images[0] = image->GetText();
			}

			if(id && name && images.size() && item && opacity && frames)
			{
				BlockDataDesc desc = { util::strToInt(id->GetText()), name->GetText(), images, item->GetText(), util::strToFloat(opacity->GetText()), (uint)util::strToInt(frames->GetText()), solid == 0 };
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

	// Create block atlas
	s_textureAtlas = new TextureAtlas();

	// Block data pixmap
	Pixmap blockDataPixmap(blockDataDesc.size(), 2);
	uchar pixelData[4];

	for(int i = 0; i < blockDataDesc.size(); i++)
	{
		BlockDataDesc *blockData = &blockDataDesc[i];

		// Create block data object
		Pixmap pixmap(blockData->images.at(0), true);
		s_nameToData[blockData->name] = s_idToData[blockData->id] = new BlockData(blockData->id, blockData->name, pixmap, blockData->itemName, blockData->opacity, blockData->solid);
		s_textureAtlas->add(util::intToStr(blockData->id), pixmap);

		// Store meta data
		pixelData[0] = blockData->frameCount;
		pixelData[1] = blockData->images.size() > 1;
		pixelData[2] = 0;
		pixelData[3] = 0;
		blockDataPixmap.setPixel(blockData->id, 1, pixelData);
	}

	s_textureAtlas->create();

	// Fill block UV data
	for(int i = 0; i < blockDataDesc.size(); ++i)
	{
		BlockDataDesc *blockData = &blockDataDesc[i];

		Vector2I pos = s_textureAtlas->get(util::intToStr(blockData->id)).uv0 * s_textureAtlas->getTexture()->getSize();

		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);

		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);

		blockDataPixmap.setPixel(blockData->id, 0, pixelData);
	}
	s_dataTexture = shared_ptr<Texture2D>(new Texture2D(blockDataPixmap));
	s_dataTexture->setFiltering(Texture2D::NEAREST);
}

BlockData::BlockData(const BlockID id, const string &name, const Pixmap &pixmap, const string &itemName, const float opacity, const bool solid) :
	m_name(name),
	m_id(id),
	m_pixmap(pixmap),
	m_itemName(itemName),
	m_opacity(opacity),
	m_solid(solid)
{
}

ItemData *BlockData::getItem() const
{
	return ItemData::getByName(m_itemName);
}
