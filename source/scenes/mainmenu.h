#ifndef MAINMENU_H
#define MAINMENU_H

#include <x2d/x2d.h>

class UiObject;
class Button;

class MainMenuScene : public XScene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

	void showWorldSelect();

private:
	vector<UiObject*> m_uiObjects;
};

#endif // MAINMENU_H