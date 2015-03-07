#include "WorldCreate.h"

#include "Gui/LineEdit.h"
#include "Gui/Button.h"
#include "Gui/Canvas.h"

#include "Game/GameObject.h"
#include "World/World.h"
#include "Constants.h"

WorldCreateScene::WorldCreateScene(World &world) :
	m_world(world)
{
	LOG("Scene: World Create");
	
	// Setup canvas
	canvas = new Canvas(800, 600);
	canvas->update();
		
	m_worldNameEdit = new LineEdit(canvas);
	m_worldNameEdit->setAnchor(Vector2(0.5f, 0.5f));
	m_worldNameEdit->setPosition(Vector2(0.0f, 0.0f));
	m_worldNameEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	m_worldNameEdit->setAcceptFunc(function<void()>(bind(&WorldCreateScene::createWorld, this))); // TODO: Accept func should probably be connected to the canvas instead
	addSceneObject(m_worldNameEdit);
	
	Button *createWorldButton = new Button("Create!", function<void()>(bind(&WorldCreateScene::createWorld, this)), canvas);
	createWorldButton->setAnchor(Vector2(0.5f, 1.0f));
	createWorldButton->setPosition(Vector2(0.0f, -0.1f));
	createWorldButton->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(createWorldButton);
}

void WorldCreateScene::createWorld()
{
	// Get world name
	string worldName = m_worldNameEdit->getText();
	if(worldName.empty()) return;

	// Create world
	m_world.create(worldName);
}