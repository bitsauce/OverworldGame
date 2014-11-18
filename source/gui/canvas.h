#ifndef CANVAS_H
#define CANVAS_H

#include <x2d/x2d.h>
#include "uiobject.h"

class Canvas : public UiObject
{
public:
	Canvas();

	void update();
};

#endif // CANVAS_H