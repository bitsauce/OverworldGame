#ifndef CAMERA_H
#define CAMERA_H

#include <x2d/x2d.h>

#include "Game/GameObject.h"

class Camera : public GameObject, public xd::MouseListener, public xd::WindowListener
{
public:
	// Constructor
	Camera();

	// Look at point
	void lookAt(Vector2i worldPoint);
	
	// Get current projection matrix
	Matrix4 getProjectionMatrix() const;

	// Get camera center
	Vector2 getCenter() const;

	// Get zoom level
	void setZoomLevel(const float zoomLevel);
	float getZoomLevel() const;

	void zoomIn() { setZoomLevel(m_zoomLevel*2.0f); }
	void zoomOut() { setZoomLevel(m_zoomLevel*0.5f); }

	// Position
	Vector2i getPosition();
	uint getX() { return getPosition().x; }
	uint getY() { return getPosition().y; }

	// Size
	void setSize(Vector2i size);
	void setSize(const uint width, const uint height) { setSize(Vector2i(width, height)); }
	Vector2i getSize() const;
	uint getWidth() const { return getSize().x; }
	uint getHeight() const { return getSize().y; }

	// Get input position
	Vector2 getInputPosition() const;

	// Update & draw
	void update();
	void draw(xd::SpriteBatch *SpriteBatch);

	// MouseListener event
	void mouseWheelEvent(const int dt);

	// WindowListener event
	void resizeEvent(uint width, uint height);

private:
	// Position
	Vector2i m_position;

	// Size
	Vector2i m_size;

	// Zoom
	float m_zoomLevel;
};

#endif // CAMERA_H