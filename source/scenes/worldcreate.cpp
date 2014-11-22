#include "worldcreate.h"

#include "gui/lineedit.h"
#include "gui/button.h"
#include "gui/canvas.h"

#include "game/gameobject.h"

#include "game.h"
#include "constants.h"

void WorldCreateScene::showEvent()
{
	LOG("Scene: World Create");
	
	// Setup canvas
	canvas = new Canvas();
	canvas->setSize(Vector2(1.0f, 1.0f));
		
	m_worldNameEdit = new LineEdit(canvas);
	m_worldNameEdit->setAnchor(Vector2(0.5f, 0.5f));
	m_worldNameEdit->setPosition(Vector2(0.0f, 0.0f));
	m_worldNameEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	m_worldNameEdit->setAcceptFunc(function<void()>(bind(&WorldCreateScene::createWorld, this))); // TODO: Accept func should probably be connected to the canvas instead
		
	Button *createWorldButton = new Button("Create!", function<void()>(bind(&WorldCreateScene::createWorld, this)), canvas);
	createWorldButton->setAnchor(Vector2(0.5f, 1.0f));
	createWorldButton->setPosition(Vector2(0.0f, -0.1f));
	createWorldButton->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));

	m_uiObjects.push_back(m_worldNameEdit);
	m_uiObjects.push_back(createWorldButton);
}

void WorldCreateScene::hideEvent()
{
	for(uint i = 0; i < m_uiObjects.size(); ++i)
	{
		delete m_uiObjects[i];
	}
	m_uiObjects.clear();
}

void WorldCreateScene::drawEvent()
{
	Game::draw();
}

void WorldCreateScene::updateEvent()
{
	Game::update();
}

void WorldCreateScene::createWorld()
{
}