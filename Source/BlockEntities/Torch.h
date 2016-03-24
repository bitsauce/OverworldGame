#pragma once

#include "BlockEntity.h"
#include "Game/Game.h"

class BlockEntityData;

class Torch : public BlockEntity
{
public:
	Torch(World * world, int x, int y, const BlockEntityData *data);

	void onNeighbourChanged(NeighborChangedEvent *e);

	static BlockEntity *Factory(World * world, const int x, const int y, const BlockEntityData *data)
	{
		return new Torch(world, x, y, data);
	}

private:
	Pointlight m_pointlight;
};