#ifndef WORLD_SELECT_H
#define WORLD_SELECT_H

#include <x2d/x2d.h>

class UiObject;
class Button;

class WorldSelectScene : public XScene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

	void worldClicked();
	void showCreateWorld();

private:
	vector<UiObject*> m_uiObjects;
};

#endif // WORLD_SELECT_H