#pragma once

#include "BlockEntity.h"
#include "Game/Game.h"

class Bush : public BlockEntity
{
public:	
	static BlockEntity *Factory(World * world, const int x, const int y, const BlockEntityData *data) { return new Bush(world, x, y, data); }
	
private:
	Bush(World * world, int x, int y, const BlockEntityData *data);
};

class Beehive : public BlockEntity
{
public:
	BLOCK_ENTITY_FACTORY(Beehive)

private:
	Beehive(World * world, int x, int y, const BlockEntityData *data);
};