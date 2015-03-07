#ifndef SCENE_H
#define SCENE_H

#include "Config.h"
#include "Gui/uiobject.h"

class Scene
{
	friend class SceneManager;
protected:
	virtual ~Scene() { for(UiObject *object : m_sceneObjects) delete object; }

	void addSceneObject(UiObject *object) { m_sceneObjects.push_back(object); }
	void removeSceneObject(UiObject *object) { m_sceneObjects.remove(object); }

private:
	list<UiObject*> m_sceneObjects;
};

#endif // SCENE_H