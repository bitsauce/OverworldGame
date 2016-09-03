#pragma once

#include "Config.h"
#include "LightSource.h"

class Pointlight : public LightSource
{
	friend class World;
public:
	Pointlight(World *world);
};