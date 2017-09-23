#pragma once

#include "Config.h"
#include "Entity.h"

class Overworld;
class Pointlight;

class DebugPointlight : public Entity
{
	friend class InGameDebug;
public:
	DebugPointlight(const Json::Value &attributes);
	~DebugPointlight();

	void onTick(TickEvent *e);

	void onSaveData(FileWriter &f);
	void onLoadData(FileReader &f);

	Pointlight *getPointlight() const;

	static Entity *Factory(const Json::Value &attributes)
	{
		return new DebugPointlight(attributes);
	}

private:
	Pointlight *m_pointlight;

	static list<DebugPointlight*> s_pointlights;
};