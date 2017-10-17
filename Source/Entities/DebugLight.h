#pragma once

#include "Config.h"
#include "Entity.h"

class Overworld;
class Pointlight;

/*class DebugPointlight : public Entity
{
	friend class InGameDebug;
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
};*/