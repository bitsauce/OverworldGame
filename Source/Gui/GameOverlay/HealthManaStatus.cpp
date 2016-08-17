#include "HealthManaStatus.h"
#include "Constants.h"
#include "Game/Game.h"
#include "GameOverlay.h"
#include "Entities/Player.h"

HealthManaStatus::HealthManaStatus(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(gameOverlay),
	m_gameOverlay(gameOverlay),
	m_heartSprite(Resource<Texture2D>("Sprites/Gui/GameOverlay/Heart")),
	m_manaSprite(Resource<Texture2D>("Sprites/Gui/GameOverlay/Mana")),
	m_heartTime(0.0f)
{
	m_heartSprite.setSize(Vector2F(32.0f));
	m_heartSprite.setOrigin(Vector2F(16.0f));
	m_heartSprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_manaSprite.setSize(Vector2F(32.0f));
	m_manaSprite.setOrigin(Vector2F(16.0f));
	m_manaSprite.getTexture()->setFiltering(Texture2D::LINEAR);

	setAnchor(Vector2F(1.0f, 0.0f));
	setSize(Vector2F(338.0f, 168.0f) / m_gameOverlay->getSize());
	setPosition(Vector2F(-48.0f, 48.0f) / m_gameOverlay->getSize());
}

void HealthManaStatus::onTick(TickEvent *e)
{
	setSize(Vector2F(338.0f, 168.0f) / m_gameOverlay->getSize());
	m_heartTime += e->getDelta();
	UiObject::onTick(e);
}

void HealthManaStatus::onDraw(DrawEvent *e)
{
	if(!m_gameOverlay->getPlayer() || !m_gameOverlay->m_active) return;

	Vector2F position = getPosition();
	Vector2F size = getSize();

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	GraphicsContext *context = e->getGraphicsContext();

	for(uint i = 0; i < m_gameOverlay->getPlayer()->getMaxHealth()/4; ++i)
	{
		uint x = i % 10, y = i / 10;
		m_heartSprite.setPosition(position + Vector2F(x * 34.0f, y * 34.0f));
		m_heartSprite.setOrigin(Vector2F(16));
		if(i == m_gameOverlay->getPlayer()->getMaxHealth() / 4 - 1)
		{
			m_heartSprite.setScale(Vector2F(sin(m_heartTime*5.0f) * 0.5f + 0.5f));
		}
		else
		{
			m_heartSprite.setScale(Vector2F(1.0f));
		}
		spriteBatch->drawSprite(m_heartSprite);
	}
}