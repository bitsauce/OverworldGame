#ifndef HEALTH_MANA_STATUS
#define HEALTH_MANA_STATUS

#include "Config.h"
#include "Gui/UiObject.h"

class Player;
class Canvas;

class HealthManaStatus : public UiObject
{
public:
	HealthManaStatus(Player *player, UiObject *parent);

	void update(const float dt);
	void draw(SpriteBatch *spriteBatch);

private:
	Player *m_player;
	Sprite m_heartSprite;
	float m_heartTime;
	Sprite m_manaSprite;
};

#endif // HEALTH_MANA_STATUS