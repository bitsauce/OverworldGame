#pragma once

#include "BlockEntity.h"
#include "Game/Game.h"

class Pot : public BlockEntity
{
public:
	Pot(World * world, int x, int y, const BlockEntityData *data);

	static BlockEntity *Factory(World * world, const int x, const int y, const BlockEntityData *data)
	{
		return new Pot(world, x, y, data);
	}

private:
};