#include "Bow.h"
#include "Constants.h"
#include "Entities/Player.h"
#include "Physics/PhysicsBody.h"
#include "World/Camera.h"
#include "World/World.h"

Bow::Bow() :
	m_bowSprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/WoodenBow.png"))
{
	m_bowSprite.setRegion(TextureRegion(), true);
}

class Arrow : public Entity
{
public:
	Arrow(World &world, const Vector2 &pos, const Vector2 &dir, const float speed) :
		Entity(world, ENTITY_ARROW),
		m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/Arrow.png")),
		m_hasHit(false),
		m_deleteTime(0.0f),
		m_body(world)
	{
		m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
		m_sprite.setRegion(TextureRegion(), true);
		m_body.setPosition(pos);
		m_body.setVelocity(dir.normalized() * speed);
		m_body.setGravityScale(0.1f);
	}

	void draw(SpriteBatch *spriteBatch)
	{
		float angle = m_hasHit ? (m_sprite.getRotation()/180.0f * PI) : atan2(m_body.getVelocity().y, m_body.getVelocity().x);
		m_sprite.setRotation(angle * (180.0f/PI));
		m_sprite.setPosition(m_body.getPosition() - Vector2(cos(angle), sin(angle)) * 16.0f + Vector2(0.0f, 3.0f));
		spriteBatch->drawSprite(m_sprite);
	}

	void update()
	{
		if(m_hasHit)
		{
			m_deleteTime += Graphics::getTimeStep();
			if(m_deleteTime > 10.0f)
			{
				delete this;
				return;
			}
			return;
		}

		m_body.update();
		if(m_body.isContact(NESW))
		{
			m_hasHit = true;
		}
	}
	
	Vector2 getPosition() const { return m_body.getPosition(); }
	Vector2 getSize() const { return m_body.getSize(); }

private:
	PhysicsBody m_body;
	Sprite m_sprite;
	bool m_hasHit;
	float m_deleteTime;
};

void Bow::use(Player *player)
{
	new Arrow(player->getWorld(), player->getBody().getCenter(), player->getWorld().getCamera()->getInputPosition() - player->getBody().getCenter(), 25.0f);
}

void Bow::draw(Player *player, SpriteBatch *spriteBatch)
{
	player->getHumanoid().drawRightHandSprite(m_bowSprite, Vector2(10.0f, 24.0f), spriteBatch);
}