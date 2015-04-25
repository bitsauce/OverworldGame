#ifndef OMNICON_H
#define OMNICON_H

#include "Config.h"
#include "Gui/UiObject.h"

class Player;

class Omnicon : public UiObject
{
public:
	Omnicon(Scene *scene, Player *player, UiObject *parent);

	void toggle();
	void setVisible(const bool visible) { m_visible = visible; }
	bool isVisible() const { return m_visible; }

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	Sprite m_omnicon;
	Player *m_player;
	bool m_visible;
};

#endif // OMNICON_H