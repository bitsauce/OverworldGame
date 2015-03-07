#include "gameobject.h"
#include "gamemanager.h"

GameObject::GameObject(Priority depth) :
	m_depth(depth)
{
	GameManager::addGameObject(this);
}

GameObject::~GameObject()
{
	GameManager::removeGameObject(this);
}