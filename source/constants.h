#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600

#define UI_MENU_BUTTON_FONT font::large
#define UI_MENU_BUTTON_TEXTURE texture::menuButton

extern class UiObject *canvas;
class MainMenu;
namespace scene
{
	extern MainMenu *mainMenu;
}

namespace font
{
	extern XFont *large;
}

namespace texture
{
	extern XTexture *menuButton;
}

#endif // CONSTANTS_H