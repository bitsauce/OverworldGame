#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Config.h"
#include "Entities/Entity.h"

class Camera;
class TimeOfDay;
class World;
class Cloud;

class Background : public Entity
{
public:
	Background(World *world, Window *window);
	~Background();

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	float getWind() const
	{
		return m_wind;
	}

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

class Cloud
{
public:
	Cloud(Random &rand, Background *background) :
		m_game(Game::GetInstance()),
		m_background(background),
		m_rand(rand)
	{
		setImage(0);
		setDepth(0.2f + m_rand.nextDouble() * 0.8f);
		setPosition((float) m_rand.nextInt(0, m_game->getWindow()->getHeight() / 2), (float) m_rand.nextInt(0, m_game->getWindow()->getWidth()));
	}

	void setImage(const int num)
	{
		m_sprite.setTexture(m_game->getResourceManager()->get<Texture2D>("Sprites/Backgrounds/Clouds/" + util::intToStr(num)));
	}

	void setDepth(const float depth)
	{
		m_sprite.setSize(m_sprite.getTexture()->getSize() * depth);
		m_depth = depth;
	}

	float getDepth() const
	{
		return m_depth;
	}

	void setPosition(const float x, const float y)
	{
		m_sprite.setPosition(x, y);
	}

	void onTick(TickEvent *e)
	{
		if(m_sprite.getX() > m_game->getWindow()->getWidth())
		{
			m_sprite.setX(-m_sprite.getWidth());
			m_sprite.setY(m_rand.nextInt(0, m_game->getWindow()->getHeight() / 2));
		}
		m_sprite.setX(m_sprite.getX() + m_background->getWind() * m_depth * e->getDelta());
	}

	void onDraw(DrawEvent *e)
	{
		SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
		spriteBatch->drawSprite(m_sprite);
	}

private:
	Random &m_rand;
	Background *m_background;
	Game *m_game;
	Sprite m_sprite;
	float m_depth;
};

#endif // BACKGROUND_H