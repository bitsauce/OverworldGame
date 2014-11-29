#include "gameobject.h"
#include "debug.h"
#include "world.h"
#include "camera.h"
#include "constants.h"

GameObject::GameObject(DrawOrder depth) :
	m_depth(depth)
{
	Game::addGameObject(this);
}

GameObject::~GameObject()
{
	Game::removeGameObject(this);
}

list<GameObject*> Game::s_gameObjects;

#include "physics/physicsbody.h"

void Game::update()
{
	if(XInput::getKeyState(XD_KEY_O))
	{
		PhysicsBody *body = new PhysicsBody();
		body->setSize(BLOCK_PXF, BLOCK_PXF);
		body->setPosition(World::getCamera()->getPosition().x + XInput::getPosition().x, World::getCamera()->getPosition().y + XInput::getPosition().y);
	}

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
	bool usingSceneMat = false;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if(DRAW_ORDER_SCENE_START < (*itr)->m_depth && DRAW_ORDER_SCENE_END > (*itr)->m_depth)
		{
			if(!usingSceneMat)
			{
				batch.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
				usingSceneMat = true;
			}
		}
		else
		{
			if(usingSceneMat)
			{
				batch.setProjectionMatrix(Matrix4());
				usingSceneMat = false;
			}
		}
		(*itr)->draw(&batch);
	}
	
	Debug::setVariable("FPS", util::intToStr(XGraphics::getFPS()));
	if(XInput::getKeyState(XD_KEY_Z))
	{
		Debug::draw(&batch);
	}

	XGraphics::renderBatch(batch);
}

void Game::addGameObject(GameObject *object)
{
	list<GameObject*>::iterator itr;
	for(itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if((*itr)->m_depth < object->m_depth)
			continue;
		break;
	}
	s_gameObjects.insert(itr, object);
}

void Game::removeGameObject(GameObject *object)
{
	s_gameObjects.remove(object);
}