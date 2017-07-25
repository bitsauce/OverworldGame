#pragma once

#include "BlockEntity.h"
#include "Game/Game.h"

class BlockEntityData;

class Torch : public BlockEntity
{
public:
	static BlockEntity *Factory(const Json::Value &attributes) { return new Torch(attributes); }

	void onNeighbourChanged(NeighborChangedEvent *e);

private:
	Torch(const Json::Value &attributes);

	void updateBaseFrame();

	Pointlight m_pointlight;
};