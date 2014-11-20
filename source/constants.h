#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600

#define UI_MENU_BUTTON_FONT "Arial Bold 27"
#define UI_MENU_BUTTON_TEXTURE ":/sprites/gui/menu_button.png"

extern class UiObject *canvas;
class MainMenuScene;
class WorldSelectScene;
class WorldCreateScene;
class GameScene;
namespace scene
{
	extern MainMenuScene *mainMenu;
	extern WorldSelectScene *worldSelect;
	extern WorldCreateScene *worldCreate;
	extern GameScene *game;
}

#endif // CONSTANTS_H