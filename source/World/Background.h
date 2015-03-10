#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Config.h"
class TimeOfDay;

class Background
{
public:
	Background(TimeOfDay *timeOfDay);

	void update(const float dt);
	void draw(SpriteBatch *SpriteBatch, const float alpha);

private:
	
	TimeOfDay *m_timeOfDay;

	Color m_topColor;
	Color m_bottomColor;
	Sprite m_sun;
	Sprite m_moon;
	
	Sprite m_hill1;
	Sprite m_hill2;

	float m_wind;
	float m_cloudTime;
	Shader *m_simplexNoise;
	Texture2D *m_cloudGradient;
	Sprite m_cloudSprite;

	Vertex m_vertices[4];
};

#endif // BACKGROUND_H