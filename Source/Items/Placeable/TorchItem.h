#pragma once

#include "BlockEntityItem.h"

class TorchItem : public BlockEntityItem
{
public:
	TorchItem(OverworldGame *game, const ItemDataDesc *desc);

	virtual void equip(Pawn *player);
	virtual void unequip(Pawn *player);
};