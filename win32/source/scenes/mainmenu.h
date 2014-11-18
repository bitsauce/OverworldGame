#ifndef MAINMENU_H
#define MAINMENU_H

#include <x2d/x2d.h>
//#include "gui/uiobject.h"
class UiObject;

class MainMenu : public XScene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

	void showWorldSelect();

private:
	vector<UiObject*> uiObjects;
};

#endif // MAINMENU_H