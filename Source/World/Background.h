#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Config.h"

class Camera;
class TimeOfDay;
class World;

class Background
{
public:
	Background(World *world);
	~Background();

	void update(const float delta);
	void draw(SpriteBatch *SpriteBatch, const float alpha);

private:
	// Manager objects
	Camera *m_camera;
	TimeOfDay *m_timeOfDay;

	struct Layer
	{
		Layer(Sprite sprite, float depth, float yOffset = 0.0f)
		{
			this->sprite = sprite;
			this->depth = depth;
			this->yOffset = yOffset;

			this->sprite.setDepth(depth);
			this->sprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
			this->sprite.getTexture()->setWrapping(Texture2D::REPEAT);
		}

		Sprite sprite;
		float depth;
		float yOffset;
		Vector2 pos, prevPos;
		Vector2 texCoord, prevTexCoord;
	};

	struct Cloud
	{
		Cloud(Sprite sprite, float depth, float initY, float initX)
		{
			this->sprite = sprite;
			this->depth = depth;
		
			this->sprite.setPosition(initX, initY);
			this->sprite.setSize(this->sprite.getTexture()->getSize() * depth);
			this->sprite.setDepth(-1.0f + depth);
		}

		Sprite sprite;
		float depth;
	};

	float m_wind;
	Shader *m_simplexNoise;
	Texture2D *m_cloudGradient;
	Sprite m_sun, m_moon;

	vector<Cloud*> m_clouds;
	vector<Layer*> m_layers;
	
	// Sky colors
	Color m_topColor, m_bottomColor;

	// Sky quad
	Vertex m_vertices[4];
};

#endif // BACKGROUND_H