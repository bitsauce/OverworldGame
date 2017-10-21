#include "BlockItem.h"
#include "Entities/Pawn.h"
#include "Animation/RegionAttachment.h"
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

BlockItem::BlockItem(World *world, Pawn *pawn, const Json::Value &attributes) :
	Item(world, pawn)
{
	m_blockData = BlockData::get(attributes.get("block", "Empty_Block").asString());
	m_layer = toWorldLayer(attributes.get("layer", "middle").asString());
}

void BlockItem::equip()
{
	RegionAttachment *attachment = m_pawn->getHumanoid().setAttachment(Humanoid::RIGHT_HAND, "Right_Hand_Equip", m_blockData->getName());
	attachment->setPosition(0.8f, 12.7f);
	attachment->setRotation(405.0f);
	attachment->setSize(24, 24);
}

void BlockItem::unequip()
{
	m_pawn->getHumanoid().clearAttachment(Humanoid::RIGHT_HAND);
}

void BlockItem::update(const float delta)
{
	// Get block input position
	Vector2I blockPos = m_world->getCamera()->getInputPosition();
	blockPos.x = (int) floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int) floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	if(m_pawn->getController()->getInputState(Controller::INPUT_USE_ITEM) &&
	   m_world->getTerrain()->setBlockAt(blockPos.x, blockPos.y, m_layer, m_blockData))
	{
		// Remove item from player inventory
		m_pawn->getCurrentItem()->dec();
	}
}

void BlockItem::draw(SpriteBatch *spriteBatch, const float alpha)
{
	//pawn->getHumanoid().drawRightHandSprite(Sprite(BlockData::getBlockAtlas()->getTexture(), Rect(0,0,16,16), Vector2(), 0.0f, BlockData::getBlockAtlas()->get(m_blockID, 0,0,1,0.6f)), Vector2(8.0f, 16.0f), spriteBatch);
}