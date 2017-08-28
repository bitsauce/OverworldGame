#ifndef HEALTH_MANA_STATUS
#define HEALTH_MANA_STATUS

#include "Config.h"
#include "Gui/UiObjects/UiObject.h"

class GameOverlay;
class Canvas;

class HealthManaStatus : public UiObject
{
public:
	HealthManaStatus(Scene *scene, GameOverlay *gameOverlay);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_heartSprite;
	float m_heartTime;
	Sprite m_manaSprite;
};

#endif // HEALTH_MANA_STATUS