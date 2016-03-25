#pragma once

#include "BlockEntity.h"
#include "Game/Game.h"

class Bush : public BlockEntity
{
public:
	Bush(World * world, int x, int y, const BlockEntityData *data);

	void onNeighbourChanged(NeighborChangedEvent *e);

	static BlockEntity *Factory(World * world, const int x, const int y, const BlockEntityData *data)
	{
		return new Bush(world, x, y, data);
	}

private:
};