#include "HealthManaStatus.h"
#include "Constants.h"
#include "Gui/Canvas.h"
#include "Entities/Player.h"

HealthManaStatus::HealthManaStatus(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player),
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
	setSize(Vector2(338.0f, 168.0f)/canvas->getSize());
	setPosition(Vector2(-48.0f, 48.0f)/canvas->getSize());
}

void HealthManaStatus::update()
{
	setSize(Vector2(338.0f, 168.0f)/canvas->getSize());
	m_heartTime += Graphics::getTimeStep();
}

void HealthManaStatus::draw(SpriteBatch *spriteBatch)
{
	Vector2 position = getPosition();
	Vector2 size = getSize();

	for(uint i = 0; i < m_player->getMaxHealth()/4; ++i)
	{
		uint x = i % 10, y = i / 10;
		m_heartSprite.setPosition(position + Vector2(x * 34, y * 34));
		if(i == m_player->getMaxHealth()/4 - 1)
		{
			m_heartSprite.setSize(Vector2(32.0f * (sin(m_heartTime*5.0f) * 0.5f + 0.5f)));
			m_heartSprite.move((Vector2(32.0f) - m_heartSprite.getSize())/2.0f);
		}
		else
		{
			m_heartSprite.setSize(Vector2(32.0f));
		}
		m_heartSprite.setOrigin(m_heartSprite.getCenter());
		spriteBatch->drawSprite(m_heartSprite);
	}
	
	/*for(int y = 0; y < 2; ++y)
	{
		for(int x = 0; x < 10; ++x)
		{
			m_manaSprite.setPosition(position + Vector2(x * 34, 102 + y * 34));
			spriteBatch->drawSprite(m_manaSprite);
		}
	}*/
}