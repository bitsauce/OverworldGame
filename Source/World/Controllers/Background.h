#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Config.h"

class Camera;
class TimeOfDay;
class World;
class Cloud;
class OverworldGame;

class Background : public SceneObject
{
	friend class Commander;
public:
	Background(World *world, Window *window);
	~Background();

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	
	float getWind() const { return m_wind; }
	float getCloudHeight() const { return m_cloudHeight; }
	float getCloudOffset() const { return m_cloudOffset; }

private:
	// Manager objects
	Camera *m_camera;
	TimeOfDay *m_timeOfDay;
	Window *m_window;

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
		Vector2F pos, prevPos;
		Vector2F texCoord, prevTexCoord;
	};

	Random m_rand;
	float m_wind, m_cloudHeight, m_cloudOffset;
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

class Cloud
{
public:
	Cloud(Random &rand, Background *background);

	void setImage(const int num);
	void setDepth(const float depth);
	float getDepth() const;

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	Random &m_rand;
	Background *m_background;
	OverworldGame *m_game;
	Sprite m_sprite;
	Vector2F m_position, m_prevPosition;
	float m_depth;
	float m_height;
};

#endif // BACKGROUND_H