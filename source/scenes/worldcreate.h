#ifndef WORLD_CREATE_H
#define WORLD_CREATE_H

#include <x2d/x2d.h>
#include "scene.h"

class UiObject;
class LineEdit;
class Button;

class WorldCreateScene : public Scene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

	void showWorldSelect();

	void createWorld();

private:
	LineEdit *m_worldNameEdit;
	vector<UiObject*> m_uiObjects;
};

#endif // WORLD_CREATE_H