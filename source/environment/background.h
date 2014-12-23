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
	void draw(xd::SpriteBatch *SpriteBatch);

private:
	
	TimeOfDay *m_timeOfDay;

	xd::Color m_topColor;
	xd::Color m_bottomColor;
	xd::Sprite m_sun;
	xd::Sprite m_moon;
	
	xd::Sprite m_hill1;
	xd::Sprite m_hill2;

	float m_wind;
	float m_cloudTime;
	xd::Shader *m_simplexNoise;
	xd::Texture2D *m_cloudGradient;
	xd::Sprite m_cloudSprite;

	xd::Vertex m_vertices[4];
};

#endif // BACKGROUND_H