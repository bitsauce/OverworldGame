#include "BlockItem.h"
#include "Entities/Pawn.h"
#include "World/World.h"
#include "Blocks/BlockData.h"
#include "Game/Game.h"

WorldLayer toWorldLayer(const string &layer)
{
	if(layer == "middle") return WORLD_LAYER_MIDDLE;
	if(layer == "back") return WORLD_LAYER_BACK;
	if(layer == "front") return WORLD_LAYER_FRONT;
	return WORLD_LAYER_MIDDLE;
}

BlockItem::BlockItem(OverworldGame *game, const ItemDataDesc *desc) :
	ItemData(desc),
	m_camera(game->getWorld()->getCamera()),
	m_terrain(game->getWorld()->getTerrain())
{
	if(desc->userData.find("layer") != desc->userData.end())
	{
		const string layer = desc->userData.at("layer");
		if(layer == "middle") { m_layer = WORLD_LAYER_MIDDLE; }
		else if(layer == "back") { m_layer = WORLD_LAYER_BACK; }
		else if(layer == "front") { m_layer = WORLD_LAYER_FRONT; }
		else
		{
			LOG("Block item 'layer' (says: %s) invalid", layer);
			m_layer = WORLD_LAYER_MIDDLE;
		}
	}
	else
	{
		LOG("Block item '%s' missing 'layer' data", desc->name.c_str());
	}

	if(desc->userData.find("block") != desc->userData.end())
	{
		const string blockName = desc->userData.at("block");
		m_blockData = BlockData::getByName(blockName);
		if(!m_blockData)
		{
			LOG("Block item 'block' (says: %s) could not be found", blockName.c_str());
		}
	}
	else
	{
		LOG("Block item '%s' missing 'block' data", desc->name.c_str());
	}
}

void BlockItem::use(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2I blockPos = m_camera->getInputPosition();
	blockPos.x = (int) floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int) floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	if(m_terrain->setBlockAt(blockPos.x, blockPos.y, m_layer, m_blockData->getID()))
	{
		// Remove item from player inventory
		pawn->getCurrentItem()->dec();
	}
}

void BlockItem::draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha)
{
	//pawn->getHumanoid().drawRightHandSprite(Sprite(BlockData::getBlockAtlas()->getTexture(), Rect(0,0,16,16), Vector2(), 0.0f, BlockData::getBlockAtlas()->get(m_blockID, 0,0,1,0.6f)), Vector2(8.0f, 16.0f), spriteBatch);
}