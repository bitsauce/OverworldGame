#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <x2d/x2d.h>

#include "game/gameobject.h"

class TimeOfDay;

class Background : public GameObject
{
public:
	Background(TimeOfDay *timeOfDay);

	void update();
	void draw(XBatch *batch);

private:
	
	TimeOfDay *m_timeOfDay;

	XColor m_topColor;
	XColor m_bottomColor;
	XSprite *m_sun;
	XSprite *m_moon;
	
	XSprite *m_hill1;
	XSprite *m_hill2;
	float m_wind;
	float m_cloudTime;
	XShader *m_simplexNoise;
	XTexture *m_cloudGradient;
	XSprite *m_cloudSprite;
};

#endif // BACKGROUND_H