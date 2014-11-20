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
}

void Game::draw()
{
	XBatch batch;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		(*itr)->draw(&batch);
	}
	XGraphics::renderBatch(batch);
}

void Game::addGameObject(GameObject *object)
{
	s_gameObjects.push_back(object);
}

void Game::removeGameObject(GameObject *object)
{
	s_gameObjects.remove(object);
}