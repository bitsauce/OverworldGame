#include "Bow.h"
#include "Constants.h"
#include "Entities/Player.h"
#include "Entities/Physics/DynamicEntity.h"
#include "World/Camera.h"
#include "World/World.h"
#include "Game/Game.h"

Bow::Bow(Game *game) :
	m_game(game),
	m_bowSprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/WoodenBow.png"))
{
	m_bowSprite.setRegion(TextureRegion(), true);
}

class Arrow : public DynamicEntity
{
public:
	Arrow(World *world, const Vector2 &pos, const Vector2 &dir, const float speed) :
		DynamicEntity(world, ENTITY_ARROW),
		m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/Arrow.png")),
		m_prevPosition(pos),
		m_hasHit(false),
		m_deleteTime(0.0f)
	{
		m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
		m_sprite.setRegion(TextureRegion(), true);
		setPosition(pos);
		setVelocity(dir.normalized() * speed);
		setGravityScale(1.0f);
		m_angle = m_prevAngle = atan2(dir.y, dir.x);
	}

	void draw(SpriteBatch *spriteBatch, const float alpha)
	{
		float angle = math::lerp(m_prevAngle, m_angle, alpha);
		m_sprite.setRotation(angle * (180.0f/PI));
		m_sprite.setPosition(math::lerp(m_prevPosition, getPosition(), alpha) - Vector2(cos(angle), sin(angle)) * 16.0f + Vector2(0.0f, 3.0f));
		spriteBatch->drawSprite(m_sprite);
	}

	void update(const float delta)
	{
		m_prevPosition = getPosition();
		if(m_hasHit)
		{
			m_deleteTime += delta;
			if(m_deleteTime > 10.0f)
			{
				delete this;
				return;
			}
			return;
		}

		if(isContact(NESW))
		{
			m_hasHit = true;
		}
		
		m_prevAngle = m_angle;
		m_angle = m_hasHit ? (m_sprite.getRotation()/180.0f * PI) : atan2(getVelocity().y, getVelocity().x);
		
		DynamicEntity::update(delta);
	}

private:
	Sprite m_sprite;
	Vector2 m_prevPosition;
	float m_angle;
	float m_prevAngle;
	bool m_hasHit;
	float m_deleteTime;
};

void Bow::use(ItemContainer::Slot *slot, const float delta)
{
	new Arrow(m_game->getWorld(), m_game->getWorld()->getLocalPlayer()->getCenter(), m_game->getWorld()->getCamera()->getInputPosition() - m_game->getWorld()->getLocalPlayer()->getCenter(), 50.0f);
}

void Bow::draw(Player *player, SpriteBatch *spriteBatch, const float alpha)
{
	player->getHumanoid().drawRightHandSprite(m_bowSprite, Vector2(10.0f, 24.0f), spriteBatch);
}