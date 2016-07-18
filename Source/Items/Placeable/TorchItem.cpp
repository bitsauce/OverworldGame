#include "TorchItem.h"
#include "Entities/Pawn.h"
#include "Animation/RegionAttachment.h"
#include "World/World.h"

TorchItem::TorchItem(OverworldGame *game, const ItemDataDesc *desc) :
	BlockEntityItem(game, desc)
{
}

void TorchItem::equip(Pawn *player)
{
	// Set "Hold Torch" animation
	player->getHumanoid().setPostAnimation(Humanoid::ANIM_HOLD_TORCH);

	// Change the player ambient light
	player->m_pointLight->setRadius(34.0f);
	player->m_pointLight->setColor(Color(216, 206, 64, 255));

	// Equip "Torch" attachment
	RegionAttachment *attachment = player->getHumanoid().setAttachment(Humanoid::HAND_RIGHT, "Right_Hand_Equip", "Torch");
	attachment->setPosition(0.0f, 7.7f);
	attachment->setRotation(0.0f);
}

void TorchItem::unequip(Pawn *player)
{
	// Reset animation
	player->getHumanoid().setPostAnimation(Humanoid::ANIM_NULL);

	// Reset player ambient light
	player->m_pointLight->setRadius(10);
	player->m_pointLight->setColor(Color(127));

	// Clear "Torch" attachment
	player->getHumanoid().clearAttachment(Humanoid::HAND_RIGHT);
}
