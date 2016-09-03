#pragma once

#include "Config.h"
#include "LightSource.h"

class Spotlight : public LightSource
{
	friend class World;
public:
	Spotlight(World *world);

	void setConeAngle(const float f);
	float getConeAngle() const;

private:
	float m_coneAngle;
};