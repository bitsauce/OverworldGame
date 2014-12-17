#include "worldcreate.h"

#include "gui/lineedit.h"
#include "gui/button.h"
#include "gui/canvas.h"

#include "game/gameobject.h"
#include "game/world.h"

#include "game.h"
#include "constants.h"

void WorldCreateScene::showEvent()
{
	LOG("Scene: World Create");
	
	// Setup canvas
	canvas = new Canvas();
	canvas->update();
		
	m_worldNameEdit = new LineEdit(canvas);
	m_worldNameEdit->setAnchor(Vector2(0.5f, 0.5f));
	m_worldNameEdit->setPosition(Vector2(0.0f, 0.0f));
	m_worldNameEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	m_worldNameEdit->setAcceptFunc(function<void()>(bind(&WorldCreateScene::createWorld, this))); // TODO: Accept func should probably be connected to the canvas instead
	m_sceneObjects.push_back(m_worldNameEdit);
	
	Button *createWorldButton = new Button("Create!", function<void()>(bind(&WorldCreateScene::createWorld, this)), canvas);
	createWorldButton->setAnchor(Vector2(0.5f, 1.0f));
	createWorldButton->setPosition(Vector2(0.0f, -0.1f));
	createWorldButton->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	m_sceneObjects.push_back(createWorldButton);
}

void WorldCreateScene::hideEvent()
{
	for(uint i = 0; i < m_sceneObjects.size(); ++i)
		delete m_sceneObjects[i];
	m_sceneObjects.clear();
}

void WorldCreateScene::createWorld()
{
	// Get world name
	string worldName = m_worldNameEdit->getText();
	if(worldName.empty()) return;

	// Create world
	World::create(worldName);
}