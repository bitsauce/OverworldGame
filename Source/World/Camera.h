#ifndef CAMERA_H
#define CAMERA_H

#include "Config.h"

class DynamicEntity;

class Camera : public MouseListener, public WindowListener
{
public:
	// Constructor
	Camera();

	// Look at point
	void lookAt(Vector2 worldPoint);

	// Entity to follow
	void setTargetEntity(DynamicEntity *target);
	DynamicEntity *getTargetEntity() const { return m_tagetEntity; }
	
	// Get current model-view matrix
	Matrix4 getModelViewMatrix(const float alpha) const;

	// Get camera center
	Vector2 getCenter(const float alpha) const;

	// Get zoom level
	void setZoomLevel(const float zoomLevel);
	float getZoomLevel() const;

	void zoomIn(int action)
	{
		if(action != GLFW_PRESS) return;
		setZoomLevel(m_zoomLevel*2.0f);
	}
	void zoomOut(int action)
	{
		if(action != GLFW_PRESS) return;
		setZoomLevel(m_zoomLevel*0.5f);
	}

	// Position
	Vector2i getPosition() const;
	void setPosition(const Vector2 &positon)
	{
		m_position = m_prevPosition = positon; 
	}
	uint getX() const { return getPosition().x; }
	uint getY() const { return getPosition().y; }

	// Size
	void setSize(Vector2i size);
	void setSize(const uint width, const uint height) { setSize(Vector2i(width, height)); }
	Vector2i getSize() const;
	uint getWidth() const { return getSize().x; }
	uint getHeight() const { return getSize().y; }

	// Get input position
	Vector2 getInputPosition() const;

	// Update
	void update(const float dt);

	// Interpolate
	void interpolate(const float alpha);

	// MouseListener event
	void mouseWheelEvent(const int delta);

	// WindowListener event
	void resizeEvent(uint width, uint height);

private:
	// Position
	Vector2 m_position, m_prevPosition, m_velocity, m_interpolatedPositon;

	// Size
	Vector2i m_size;

	// Zoom
	float m_zoomLevel;

	// Target entity
	DynamicEntity *m_tagetEntity;
};

#endif // CAMERA_H