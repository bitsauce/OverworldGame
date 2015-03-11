#ifndef SCENE_H
#define SCENE_H

#include "Config.h"
#include "Gui/uiobject.h"

class Scene
{
	friend class SceneManager;
	friend class UiObject;
protected:
	virtual ~Scene() { for(UiObject *object : m_sceneObjects) delete object; }

	static void addUiObject(UiObject *object) { m_sceneObjects.push_back(object); }
	static void removeUiObject(UiObject *object) { m_sceneObjects.remove(object); }

public:
	static list<UiObject*> getUiObjects() { return m_sceneObjects; }

private:
	static list<UiObject*> m_sceneObjects;
};

#endif // SCENE_H