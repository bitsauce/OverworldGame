#pragma once

#include "UiObject.h"

class Canvas : public UiObject
{
	friend class Gui;
public:
	Canvas(Window *window, const bool transparent = false);

	void onWindowSizeChanged(WindowEvent *e);
};