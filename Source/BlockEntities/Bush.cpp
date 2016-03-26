#include "Bush.h"

Bush::Bush(World *world, int x, int y, const BlockEntityData *data) :
	BlockEntity(world, x, y, data)
{
}

void Bush::onNeighbourChanged(NeighborChangedEvent *e)
{
	BlockEntity::onNeighbourChanged(e);
}