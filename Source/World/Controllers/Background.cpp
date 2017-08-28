#include "Game/Game.h"
#include "Background.h"
#include "TimeOfDay.h"
#include "Constants.h"
#include "World/World.h"

struct CloudSort
{
	inline bool operator() (Cloud *cloud1, Cloud *cloud2)
	{
		return (cloud1->getDepth() < cloud2->getDepth());
	}
};

Background::Background(World *world, Window *window) :
	m_camera(world->getCamera()),
	m_window(window),
	m_timeOfDay(world->getTimeOfDay()),
	m_topColor(255, 255, 255),
	m_bottomColor(90, 170, 255),
	m_wind(5.0f),
	m_cloudHeight(0.0f),
	m_cloudOffset(500.0f),
	m_sun(Resource<Texture2D>("Sprites/Backgrounds/Sun")),
	m_moon(Resource<Texture2D>("Sprites/Backgrounds/Moon"))
{
	m_sun.setSize(m_sun.getTexture()->getSize());
	m_moon.setSize(m_moon.getTexture()->getSize());

	m_sun.setOrigin(m_sun.getCenter());
	m_moon.setOrigin(m_moon.getCenter());

	m_sun.setDepth(-2.0f);
	m_moon.setDepth(-2.0f);

	for(uint i = 0; i < 10; ++i)
	{
		m_clouds.push_back(new Cloud(m_rand, this));
	}
	sort(m_clouds.begin(), m_clouds.end(), CloudSort());

	m_layers.push_back(new Layer(Sprite(Resource<Texture2D>("Sprites/Backgrounds/Layer/0")), 0.5f, -1080.0f));
	m_layers.push_back(new Layer(Sprite(Resource<Texture2D>("Sprites/Backgrounds/Layer/1")), 0.25f));
	m_layers.push_back(new Layer(Sprite(Resource<Texture2D>("Sprites/Backgrounds/Layer/2")), 0.125f, -100.0f));
}

Background::~Background()
{
	for(Layer *layer : m_layers)
	{
		delete layer;
	}
	m_layers.clear();
}

void Background::onTick(TickEvent *e)
{
	// Get hour and mintue
	int hour = m_timeOfDay->getHour();
	float time = m_timeOfDay->getTime();

	// Change background depending on time
	if(m_timeOfDay->isDay())
	{
		// Apply sunrise from 6:00 to 9:00
		if(hour >= 6 && hour < 9)
		{
			// Percentage of sunrise
			float minscale = 1.0f - (540 - time) / 180.0f; // Aka. (9 * 60 - time) / (6 * 60 - 9 * 60)
			m_topColor = math::lerp(ColorRGB(255, 255, 255), ColorRGB(0, 0, 0), minscale);
			m_bottomColor = math::lerp(ColorRGB(90, 170, 255), ColorRGB(10, 60, 110), minscale);
		}
		else
		{
			// Set day gradient
			m_topColor = ColorRGB(255, 255, 255);
			m_bottomColor = ColorRGB(90, 170, 255);
		}

		// Place sun
		float ang = (time - 360.0f) / 720.0f;
		Vector2F windowSize = m_window->getSize();
		Vector2F sunSize = m_sun.getSize();
		m_sun.setPosition(
			windowSize.x * 0.5f - cos(PI * ang) * (windowSize.x * 0.5f + sunSize.x),
			windowSize.y - sin(PI * ang) * windowSize.y + sunSize.y * 0.5f
			);
		m_sun.setRotation(180 * ang);
	}
	else
	{
		// Apply sunset from 18:00 to 21:00
		if(hour >= 18 && hour < 21)
		{
			// Percentage of sunset
			float minscale = 1.0f - (1260 - time) / 180.0f; // Aka. (21 * 60 - time) / (18 * 60 - 21 * 60)
			m_topColor = math::lerp(ColorRGB(0, 0, 0), ColorRGB(255, 255, 255), minscale);
			m_bottomColor = math::lerp(ColorRGB(10, 60, 110), ColorRGB(90, 170, 255), minscale);
		}
		else
		{
			// Set night gradient
			m_topColor = ColorRGB(0, 0, 0);
			m_bottomColor = ColorRGB(10, 60, 110);
		}

		// Place moon
		float ang = (time > 360.0f ? (time - 1080.0f) : (time + 360.0f)) / 720.0f;
		Vector2F windowSize = m_window->getSize();
		Vector2F moonSize = m_moon.getSize();
		m_moon.setPosition(
			windowSize.x * 0.5f - cos(PI * ang) * (windowSize.x * 0.5f + moonSize.x),
			windowSize.y - sin(PI * ang) * windowSize.y + moonSize.y * 0.5f
			);
		m_moon.setRotation(180 * ang);
	}

	// Apply wind
	for(Cloud *cloud : m_clouds)
	{
		cloud->onTick(e);
	}
}

void Background::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();

	// Draw sky gradient
	m_vertices[0].set2f(VERTEX_POSITION, 0.0f, 0.0f);
	m_vertices[1].set2f(VERTEX_POSITION, 0.0f, (float) graphicsContext->getHeight());
	m_vertices[2].set2f(VERTEX_POSITION, (float) graphicsContext->getWidth(), 0.0f);
	m_vertices[3].set2f(VERTEX_POSITION, (float) graphicsContext->getWidth(), (float) graphicsContext->getHeight());
	m_vertices[0].set4ub(VERTEX_COLOR, m_topColor.getR(), m_topColor.getG(), m_topColor.getB(), 255);
	m_vertices[1].set4ub(VERTEX_COLOR, m_bottomColor.getR(), m_bottomColor.getG(), m_bottomColor.getB(), 255);
	m_vertices[2].set4ub(VERTEX_COLOR, m_topColor.getR(), m_topColor.getG(), m_topColor.getB(), 255);
	m_vertices[3].set4ub(VERTEX_COLOR, m_bottomColor.getR(), m_bottomColor.getG(), m_bottomColor.getB(), 255);
	graphicsContext->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, m_vertices, 4);

	// Draw sun/moon
	int hour = m_timeOfDay->getHour();
	if(hour >= 6 && hour < 18)
	{
		spriteBatch->drawSprite(m_sun);
	}
	else
	{
		spriteBatch->drawSprite(m_moon);
	}

	// Draw background layers
	/*for(Layer *layer : m_layers)
	{
		float ratio = m_window->getWidth() / 1920.0f;
		Vector2I cameraPos = m_camera->getCenter(e->getAlpha());
		Vector2I layerSize = Vector2I(m_window->getWidth(), (int) (layer->sprite.getTexture()->getHeight() * ratio));
		layer->sprite.setSize(layerSize);
		if(cameraPos.y > 0.0f)
		{
			layer->sprite.setPosition(0.0f, -cameraPos.y + m_window->getHeight() * 0.5f - layerSize.y - layer->yOffset * ratio);
		}
		else
		{
			layer->sprite.setPosition(0.0f, -cameraPos.y * layer->depth * 0.1f + m_window->getHeight() * 0.5f - layerSize.y - layer->yOffset * ratio);
		}
		layer->sprite.setRegion(TextureRegion((float) (cameraPos.x * layer->depth) / (float) layerSize.x, 0.0f, (float) (cameraPos.x * layer->depth + layerSize.x) / (float) layerSize.x, 1.0f));
		spriteBatch->drawSprite(layer->sprite);
	}*/

	// Draw clouds
	for(Cloud *cloud : m_clouds)
	{
		cloud->onDraw(e);
	}
}

Cloud::Cloud(Random &rand, Background *background) :
	m_game((Overworld*) Game::Get()),
	m_background(background),
	m_rand(rand)
{
	setImage(0);
	setDepth(0.2f + m_rand.nextDouble() * 0.8f);
	m_position.x = m_prevPosition.x = (float) m_rand.nextInt(0, m_game->getWindow()->getWidth());
	m_height = m_rand.nextDouble();
}

void Cloud::setImage(const int num)
{
	m_sprite.setTexture(Resource<Texture2D>("Sprites/Backgrounds/Clouds/" + util::intToStr(num)));
}

void Cloud::setDepth(const float depth)
{
	m_sprite.setSize(m_sprite.getTexture()->getSize() * depth);
	m_depth = depth;
}

float Cloud::getDepth() const
{
	return m_depth;
}

void Cloud::onTick(TickEvent *e)
{
	m_prevPosition = m_position;

	// Adjust X
	if(m_position.x > m_game->getWindow()->getWidth())
	{
		m_position.x = m_prevPosition.x = -m_sprite.getWidth();
		m_position.y = m_rand.nextInt(0, m_game->getWindow()->getHeight() / 2);
	}
	m_position.x += m_background->getWind() * m_depth * e->getDelta();

	// Adjust Y
	m_position.y = m_background->getCloudHeight() - (m_game->getClient()->getWorld()->getCamera()->getY() + m_background->getCloudOffset()) * m_depth * 0.05f;
}

void Cloud::onDraw(DrawEvent *e)
{
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	m_sprite.setPosition(/*math::lerp(m_prevPosition, */m_position/*, e->getAlpha())*/);
	spriteBatch->drawSprite(m_sprite);
}