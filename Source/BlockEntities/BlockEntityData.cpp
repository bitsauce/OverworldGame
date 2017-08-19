#include "Constants.h"
#include "BlockEntityData.h"

#include "Stick.h"
#include "Torch.h"
#include "Bush.h"
#include "Pot.h"

map<string, BlockEntityData*> BlockEntityData::s_nameToData;
map<EntityID, BlockEntityData*> BlockEntityData::s_idToData;
TextureAtlas *BlockEntityData::s_textureAtlas;
shared_ptr<Texture2D> BlockEntityData::s_dataTexture = nullptr;

const BlockEntityFactory getFactory(const string &entityName)
{
	if (entityName == "torch") return Torch::Factory;
	return 0;
}

void BlockEntityData::init()
{
	LOG("Loading block entity data...");

	// Load block entity data from file
	if (!util::fileExists("BlockEntityData.json"))
	{
		THROW("EntityData.xml is missing!");
	}

	// Read block entity JSON data
	Json::Value blockEntitiesJSON;
	{
		Json::Reader reader;
		ifstream blockEntityDataFile("BlockEntityData.json");
		if(blockEntityDataFile.is_open())
		{
			if(!reader.parse(blockEntityDataFile, blockEntitiesJSON))
			{
				THROW("Error parsing BlockEntityData.json!");
			}
		}
		else
		{
			THROW("Error opening BlockEntityData.json!");
		}
	}

	// Create block entity texture atlas
	s_textureAtlas = new TextureAtlas();
	uint blockEntityCount = 0;

	// Load block entity data
	for(Json::Value::const_iterator itr = blockEntitiesJSON.begin(); itr != blockEntitiesJSON.end(); itr++)
	{
		const Json::Value &blockEntityJSON = *itr;
		const string name = itr.key().asString();
		const BlockEntityID id = blockEntityJSON.get("id", -1).asInt();
		const int width = blockEntityJSON.get("width", -1).asInt();
		const int height = blockEntityJSON.get("height", -1).asInt();
		const string vboGroup = blockEntityJSON.get("vboGroup", "").asString();
		const string texture = blockEntityJSON.get("texture", "").asString();
		const BlockEntityFactory factory = getFactory(name);
		uint placementRule = 0;
		for(Json::Value rule : blockEntityJSON["placementRules"])
		{
			const string ruleName = rule.asString();
			if(ruleName == "wall")            placementRule |= NEED_WALL;
			else if(ruleName == "floor")      placementRule |= NEED_FLOOR;
			else if(ruleName == "background") placementRule |= NEED_BACK_BLOCK;
			else if(ruleName == "roof")       placementRule |= NEED_ROOF;
		}
		Json::Value animation = blockEntityJSON["animation"];
		uint frameColumns = 1, frameRows = 1;
		if(!animation.empty())
		{
			frameColumns = animation.get("frameColumns", 1).asInt();
			frameRows = animation.get("frameRows", 1).asInt();
		}
		
		Pixmap pixmap(texture, true);
		assert(s_idToData.find(id) == s_idToData.end());
		assert(s_nameToData.find(name) == s_nameToData.end());
		s_nameToData[name] = s_idToData[id] = new BlockEntityData(id, name, pixmap, width, height, frameColumns, frameRows, WORLD_LAYER_MIDDLE, placementRule, factory);
		s_textureAtlas->add(util::intToStr(id), pixmap);
		blockEntityCount++;
	}

	// Generate texture atlas
	s_textureAtlas->create();

	// Block entity data pixmap
	Pixmap blockDataPixmap(blockEntityCount + 1, 2);
	uchar pixelData[4];
	for(map<BlockEntityID, BlockEntityData*>::const_iterator itr = s_idToData.begin(); itr != s_idToData.end(); itr++)
	{
		// Create block entity data objects
		BlockEntityData *data = itr->second;
		pixelData[0] = data->m_width * BLOCK_PXF;
		pixelData[1] = data->m_height * BLOCK_PXF;
		pixelData[2] = data->m_frameColumns;
		pixelData[3] = data->m_frameRows;
		blockDataPixmap.setPixel(itr->first, 1, pixelData);

		// Write UV data to texture
		Vector2I pos = s_textureAtlas->get(util::intToStr(itr->first)).uv0 * s_textureAtlas->getTexture()->getSize();
		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);
		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);
		blockDataPixmap.setPixel(itr->first, 0, pixelData);
	}
	s_dataTexture = shared_ptr<Texture2D>(new Texture2D(blockDataPixmap));
	s_dataTexture->setFiltering(Texture2D::NEAREST);
}

bool BlockEntityData::isValidPlacement(const int x, const int y, Terrain *terrain, BlockEntity *ignoreThis) const
{
	// Check that the required space is available
	for(int y1 = y; y1 < y + m_height; y1++)
	{
		for(int x1 = x; x1 < x + m_width; x1++)
		{
			if(terrain->getBlockEntityAt(x1, y1, m_layer) != ignoreThis || (m_layer == WORLD_LAYER_MIDDLE && terrain->isBlockAt(x1, y1, m_layer)))
			{
				return false;
			}
		}
	}

	// Check if we have floor
	if((m_placement & NEED_FLOOR) != 0)
	{
		bool valid = true;
		for(int x1 = x; x1 < x + m_width; x1++)
		{
			if(!terrain->isBlockAt(x1, y + m_height, m_layer))
			{
				valid = false; break;
			}
		}
		if(valid) return true;
	}

	// Check if we have walls
	if((m_placement & NEED_WALL) != 0)
	{
		// Left
		bool valid = true;
		for(int y1 = y; y1 < y + m_height; y1++)
		{
			if(!terrain->isBlockAt(x - 1, y1, m_layer))
			{
				valid = false; break;
			}
		}
		if(valid) return true;

		// Right
		valid = true;
		for(int y1 = y; y1 < y + m_height; y1++)
		{
			if(!terrain->isBlockAt(x + m_width, y1, m_layer))
			{
				valid = false; break;
			}
		}
		if(valid) return true;
	}

	// Check if we have roof
	if((m_placement & NEED_ROOF) != 0)
	{
		bool valid = true;
		for(int x1 = x; x1 < x + m_width; x1++)
		{
			if(!terrain->isBlockAt(x1, y - 1, m_layer))
			{
				valid = false; break;
			}
		}
		if(valid) return true;
	}

	// Check if we have back blocks
	if((m_placement & NEED_BACK_BLOCK) != 0)
	{
		bool valid = true;
		for(int y1 = y; y1 < y + m_height; y1++)
		{
			for(int x1 = x; x1 < x + m_width; x1++)
			{
				if(!terrain->isBlockAt(x1, y1, WORLD_LAYER_BACK))
				{
					valid = false; break;
				}
			}
		}
		if(valid) return true;
	}

	return false;
}