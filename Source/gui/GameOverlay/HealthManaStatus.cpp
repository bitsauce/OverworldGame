#include "HealthManaStatus.h"
#include "Constants.h"
#include "Game/Game.h"
#include "Entities/Player.h"

HealthManaStatus::HealthManaStatus(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_gameOverlay(gameOverlay),
	m_heartSprite(ResourceManager::get<Texture2D>(":/Sprites/Gui/GameOverlay/Heart.png")),
	m_manaSprite(ResourceManager::get<Texture2D>(":/Sprites/Gui/GameOverlay/Mana.png")),
	m_heartTime(0.0f)
{
	m_heartSprite.setSize(Vector2(32.0f));
	m_heartSprite.setOrigin(Vector2(16.0f));
	m_heartSprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_manaSprite.setSize(Vector2(32.0f));
	m_manaSprite.setOrigin(Vector2(16.0f));
	m_manaSprite.getTexture()->setFiltering(Texture2D::LINEAR);

	setAnchor(Vector2(1.0f, 0.0f));
	setSize(Vector2(338.0f, 168.0f)/m_parent->getSize());
	setPosition(Vector2(-48.0f, 48.0f)/m_parent->getSize());
}

void HealthManaStatus::update(const float delta)
{
	setSize(Vector2(338.0f, 168.0f)/m_parent->getSize());
	m_heartTime += delta;
	UiObject::update(delta);
}

void HealthManaStatus::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(!m_gameOverlay->getPlayer()) return;

	Vector2 position = getPosition();
	Vector2 size = getSize();

	for(uint i = 0; i < m_gameOverlay->getPlayer()->getMaxHealth()/4; ++i)
	{
		uint x = i % 10, y = i / 10;
		m_heartSprite.setPosition(position + Vector2(x * 34, y * 34));
		m_heartSprite.setOrigin(Vector2(16));
		if(i == m_gameOverlay->getPlayer()->getMaxHealth() / 4 - 1)
		{
			m_heartSprite.setScale(Vector2(sin(m_heartTime*5.0f) * 0.5f + 0.5f));
		}
		else
		{
			m_heartSprite.setScale(Vector2(1.0f));
		}
		spriteBatch->drawSprite(m_heartSprite);
	}
}