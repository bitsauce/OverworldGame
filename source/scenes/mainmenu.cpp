#include "mainmenu.h"
#include "gui/button.h"
#include "game.h"
#include "constants.h"

int TITLE_TILES[4][37] = {
	{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0 }
};

void MainMenu::showEvent()
{
	LOG("Scene: Main menu");

	Button *spBtn = new Button("Singleplayer", function<void()>(bind(&MainMenu::showWorldSelect, this)), canvas);
	Button *mpBtn = new Button("Multiplayer", function<void()>(), canvas);
	
	spBtn->setPosition(Vector2(0.0f, -0.1f));
	spBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	spBtn->setAnchor(Vector2(0.5f, 0.5f));

	mpBtn->setPosition(Vector2(0.0f, 0.0f));
	mpBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	mpBtn->setAnchor(Vector2(0.5f, 0.5f));
	
	uiObjects.push_back(spBtn);
	uiObjects.push_back(mpBtn);
}

void MainMenu::hideEvent()
{
	for(int i = 0; i < uiObjects.size(); ++i)
	{
		delete uiObjects[i];
	}
}

void MainMenu::drawEvent()
{
	XBatch batch;

	canvas->draw(&batch);

	// Draw all ui objects
	for(uint i = 0; i < uiObjects.size(); ++i)
	{
		uiObjects[i]->draw(&batch);
	}

	XGraphics::renderBatch(batch);

	//gameDraw();
}

void MainMenu::updateEvent()
{
	canvas->update();

	// Update all ui objects
	for(uint i = 0; i < uiObjects.size(); ++i)
	{
		uiObjects[i]->update();
	}

	gameUpdate();
}

void MainMenu::showWorldSelect()
{
	XEngine::exit();
}