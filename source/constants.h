#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600

extern class UiObject *canvas;

namespace font
{
	extern XFont *large;
}

namespace texture
{
	extern XTexture *menuButton;
}

class MainMenu;

namespace scene
{
	extern MainMenu *mainMenu;
}

#endif // CONSTANTS_H