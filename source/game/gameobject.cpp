#include "gameobject.h"
#include "gamemanager.h"

GameObject::GameObject(DrawOrder depth) :
	m_depth(depth)
{
	GameManager::addGameObject(this);
}

GameObject::~GameObject()
{
	GameManager::removeGameObject(this);
}