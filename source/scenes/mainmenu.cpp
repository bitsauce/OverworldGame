#include "mainmenu.h"
#include "gui/button.h"
#include "game.h"

int TITLE_TILES[4][37] = {
	{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0 }
};

void MainMenu::showEvent()
{
	LOG("Scene: Main menu");

	Button *spBtn = new Button("Singleplayer", &delegate<void()>(bind(&MainMenu::showWorldSelect, this)));
	
	spBtn->setPosition(Vector2(0.0f, -0.1f));
	spBtn->setSize(Vector2(300.0f/XWindow::getSize().x, 70.0f/XWindow::getSize().y));
	spBtn->setAnchor(Vector2(0.5f, 0.5f));

	uiObjects.push_back(spBtn);
}

void MainMenu::hideEvent()
{
}

void MainMenu::drawEvent()
{
	XBatch batch;

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
	// Update all ui objects
	for(uint i = 0; i < uiObjects.size(); ++i)
	{
		uiObjects[i]->update();
	}

	gameUpdate();
}

void MainMenu::showWorldSelect()
{
	LOG("Show World Select");
}