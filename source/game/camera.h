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

	// Position
	Vector2 getPosition();
	uint getX();
	uint getY();

	// Size
	Vector2i getSize();
	uint getWidth();
	uint getHeight();

	// Zoomed size
	Vector2i getSizeZoomed();
	uint getWidthZoomed();
	uint getHeightZoomed();

	// Get input position
	Vector2 getInputPosition() const;

	// Update & draw
	void update();
	void draw(xd::SpriteBatch *SpriteBatch);

	// MouseListener event
	void mouseWheelEvent(const int dt);

private:
	// Position
	Vector2i m_position;

	// Size
	Vector2i m_size;

	// Zoom
	float m_zoom;
};

#endif // CAMERA_H