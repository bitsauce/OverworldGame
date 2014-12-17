#ifndef SCENE_H
#define SCENE_H

class Scene
{
	friend class SceneManager;
protected:
	virtual void showEvent() { }
	virtual void hideEvent() { }
	//virtual void drawEvent() { }
	//virtual void updateEvent() { }
};

#endif // SCENE_H