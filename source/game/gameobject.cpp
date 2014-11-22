#include "gameobject.h"

GameObject::GameObject()
{
	Game::addGameObject(this);
}

GameObject::~GameObject()
{
	Game::removeGameObject(this);
}

list<GameObject*> Game::s_gameObjects;

void Game::update()
{
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		(*itr)->update();
	}

	/*
	// Step Box2D
	Box2D.step(Graphics.dt);
	
	// Update all managers
	Terrain.update();
	TimeOfDay.update();
	Background.update();
	Spawner.update();
	//Water.update();
	World.update();*/
}

void Game::draw()
{
	XBatch batch;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		(*itr)->draw(&batch);
	}
	XGraphics::renderBatch(batch);

	/*
	// Create translation matrix
	Matrix4 projmat = Camera.getProjectionMatrix();
	Layers[LAYER_SCENE].setProjectionMatrix(projmat);
	
	// Draw the background
	Background.draw(@Layers[LAYER_BACKGROUND]);
	//Water.draw();
	
	// Draw world content
	World.draw();
	
	// Render background
	Layers[LAYER_BACKGROUND].draw();
	
	// Draw terrain
	Terrain.draw(TERRAIN_BACKGROUND, @Layers[LAYER_BACKGROUND]);
	
	// Draw shadows
	Shadows.setProjectionMatrix(Camera.getProjectionMatrix());
	if(!(Input.getKeyState(KEY_Z) && Input.getKeyState(KEY_X))) // debug
		Shadows.draw();
	Shadows.clear();
	
	// Render scene
	Layers[LAYER_SCENE].draw();
	
	// Draw debug info
	if(Input.getKeyState(KEY_Z))
	{
		Debug.draw();
		if(Input.getKeyState(KEY_W))
			Graphics.enableWireframe();
		else
			Graphics.disableWireframe();
		if(Input.getKeyState(KEY_B))
			Box2D.draw(@Layers[LAYER_SCENE]);
	
		// Set FPS debug variable
		Debug.setVariable("FPS", ""+Graphics.FPS);
	}
	
	// Draw remaining layers
	for(uint i = LAYER_FOREGROUND; i < LAYER_COUNT; ++i) {
		Layers[i].draw();
	}
	
	// Clear layers
	for(uint i = 0; i < LAYER_COUNT; ++i) {
		Layers[i].clear();*/
}

void Game::addGameObject(GameObject *object)
{
	s_gameObjects.push_back(object);
}

void Game::removeGameObject(GameObject *object)
{
	s_gameObjects.remove(object);
}