#ifndef SCENE_H
#define SCENE_H

#include "Config.h"
#include "Game/GameObject.h"

class Scene
{
	friend class SceneManager;
protected:
	virtual ~Scene() { for(GameObject *object : m_sceneObjects) delete object; }

	void addSceneObject(GameObject *object) { m_sceneObjects.push_back(object); }
	void removeSceneObject(GameObject *object) { m_sceneObjects.remove(object); }

private:
	list<GameObject*> m_sceneObjects;
};

#endif // SCENE_H