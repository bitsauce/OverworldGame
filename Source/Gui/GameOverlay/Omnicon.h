#ifndef OMNICON_H
#define OMNICON_H

#include "Config.h"
#include "Gui/UiObjects/UiObject.h"

class GameOverlay;

class Omnicon : public UiObject
{
public:
	Omnicon(Scene *scene, GameOverlay *gameOverlay);

	void toggle(int action);
	void setVisible(const bool visible) { m_visible = visible; }
	bool isVisible() const { return m_visible; }

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_bookSprite;
	bool m_visible;
};

#endif // OMNICON_H