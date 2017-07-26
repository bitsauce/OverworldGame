#include "Constants.h"
#include "BlockEntityData.h"

#include "Stick.h"
#include "Torch.h"
#include "Bush.h"
#include "Pot.h"

vector<BlockEntityData*> BlockEntityData::s_data(BLOCK_ENTITY_COUNT);
TextureAtlas *BlockEntityData::s_textureAtlas;
shared_ptr<Texture2D> BlockEntityData::s_dataTexture = nullptr;

const BlockEntityFactory getFactory(const string &entityName)
{
	if (entityName == "Torch") return Torch::Factory;
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

	// Block entity data pixmap
	Pixmap blockDataPixmap(BLOCK_ENTITY_COUNT, 2);
	uchar pixelData[4];

	// Create block entity texture atlas
	s_textureAtlas = new TextureAtlas();

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

	// Load block entity data
	for(auto &blockEntityJSON : blockEntitiesJSON)
	{
		const int id = blockEntityJSON.get("id", -1).asInt();
		const string vboGroup = blockEntityJSON.get("vboGroup", "").asString();
		const string texture = blockEntityJSON.get("texture", "").asString();
		for(auto &rule : blockEntityJSON["placementRules"])
		{
			LOG("%s", rule.asString().c_str());
		}
		
		//new BlockEntityData(id, blockEntityJSON);
	}
	
	/*BlockEntityDescriptor *data = &g_blockEntityData[0];
	vector<Pixmap> pixmaps(BLOCK_ENTITY_COUNT);
	while (data->id != BLOCK_ENTITY_COUNT)
	{
		// Create block entity data objects
		Pixmap pixmap(data->texturePath);
		s_data[data->id] = new BlockEntityData(data->id, data->name, pixmap, data->width, data->height, data->frameCount, data->layer, data->placement, data->factory);
		s_textureAtlas->add(util::intToStr(data->id), pixmap);

		pixelData[0] = data->width * BLOCK_PXF;
		pixelData[1] = data->height * BLOCK_PXF;
		pixelData[2] = data->frameCount;
		pixelData[3] = data->animationSpeed;
		blockDataPixmap.setPixel(data->id, 1, pixelData);

		// Next block entity
		data++;
	}

	// Fill block entity UV data
	for (uint i = 0; i < BLOCK_ENTITY_COUNT; ++i)
	{
		// TODO: Old code
		Vector2I pos = s_textureAtlas->get(util::intToStr(i)).uv0 * s_textureAtlas->getTexture()->getSize();

		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);

		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);

		blockDataPixmap.setPixel(i, 0, pixelData);
	}
	s_dataTexture = shared_ptr<Texture2D>(new Texture2D(blockDataPixmap));
	s_dataTexture->setFiltering(Texture2D::NEAREST);*/
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