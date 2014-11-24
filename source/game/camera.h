#ifndef CAMERA_H
#define CAMERA_H

#include <x2d/x2d.h>

#include "game/gameobject.h"

class Camera : public GameObject, public xd::MouseListener
{
public:
	Camera();
	
	Matrix4 getProjectionMatrix();
	Vector2 getCenter();
	void lookAt(Vector2 point);

	float getZoom() { return m_zoom; }

	Vector2 getPosition();
	float getX();
	float getY();
	
	Vector2 getSize();
	float getWidth();
	float getHeight();

	void update();
	void draw(XBatch *batch);
	void mouseWheelEvent(const int dt);

private:
	Vector2 m_position;
	Vector2 m_size;
	float m_zoom;
};

#endif // CAMERA_H