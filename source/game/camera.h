#ifndef CAMERA_H
#define CAMERA_H

#include <x2d/x2d.h>

#include "game/gameobject.h"

class Camera : public GameObject, public xd::MouseListener
{
public:
	// Constructor
	Camera();

	// Look at point
	void lookAt(Vector2 point);
	
	// Get current projection matrix
	Matrix4 getProjectionMatrix() const;

	// Get camera center
	Vector2 getCenter() const;

	// Get active chunk area
	Recti getActiveChunkArea() const;

	// Get zoom level
	float getZoom() const;

	// Camera position
	Vector2 getPosition();
	float getX();
	float getY();

	// Camera size
	Vector2 getSize();
	float getWidth();
	float getHeight();

	// Update & draw
	void update();
	void draw(xd::SpriteBatch *SpriteBatch);

	// MouseListener event
	void mouseWheelEvent(const int dt);

private:
	// Camera position
	Vector2 m_position;

	// Camera size (viewport size)
	Vector2 m_size;

	// Zoom level
	float m_zoom;

	// Active chunk area (chunks within viewspace)
	Recti m_activeChunkArea;
};

#endif // CAMERA_H