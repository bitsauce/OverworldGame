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

	void setDirection(const float f);
	float getDirection() const;

private:
	float m_coneAngle;
	float m_direction;
};