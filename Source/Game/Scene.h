#ifndef SCENE_H
#define SCENE_H

#include "Config.h"
#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
//>REMOVE ME<
class Scene
{
	friend class UiObject;
public:
	Scene() { m_canvas = new Canvas(this); }
	Scene(const uint w, const uint h) { m_canvas = new Canvas(this, w, h); }
	~Scene() { for(UiObject *object : m_uiObjects) delete object; }

	Canvas *getCanvas() const { return m_canvas; }

	void update(const float delta) { for(UiObject *object : m_uiObjects) object->update(delta); }
	void draw(SpriteBatch *spriteBatch, const float alpha)
	{ 
		spriteBatch->begin();
		for(UiObject *object : m_uiObjects)
		{
			object->draw(spriteBatch, alpha);
		}
		spriteBatch->end();
	}

	list<UiObject*> getUiObjects() { return m_uiObjects; }

protected:
	void addUiObject(UiObject *object) { m_uiObjects.push_back(object); }
	void removeUiObject(UiObject *object) { m_uiObjects.remove(object); }

private:
	Canvas *m_canvas;
	list<UiObject*> m_uiObjects;
};

#endif // SCENE_H