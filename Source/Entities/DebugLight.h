#pragma once

#include "Config.h"
#include "Controller.h"
#include "Networking/NetworkObject.h"
#include "Entity.h"

class OverworldGame;
class Pointlight;

class DebugPointlight : public Entity
{
	friend class Debug;
public:
	DebugPointlight(World *world);
	~DebugPointlight();

	void onTick(TickEvent *e);

	void onSaveData(FileWriter &f);
	void onLoadData(FileReader &f);

	Pointlight *getPointlight() const;

	static Entity *Factory(World *world)
	{
		return new DebugPointlight(world);
	}

private:
	Pointlight *m_pointlight;

	static list<DebugPointlight*> s_pointlights;
};