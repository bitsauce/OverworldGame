#ifndef CAMERA_H
#define CAMERA_H

#include "Config.h"

class Entity;
class World;

class Camera : public SceneObject
{
public:
	// Constructor
	Camera(World *world, InputManager *input, Window *window);

	// Look at point
	void lookAt(Vector2F worldPoint);

	// Entity to follow
	void setTargetEntity(Entity *target);
	Entity *getTargetEntity() const { return m_tagetEntity; }
	
	// Get current model-view matrix
	Matrix4 getTransformationMatrix(const float alpha) const;

	// Get camera center
	Vector2I getCenter(const float alpha) const;

	// Get zoom level
	void setZoomLevel(const float zoomLevel);
	float getZoomLevel() const;

	void zoomIn(InputEvent *e);
	void zoomOut(InputEvent *e);

	// Position
	void setPosition(const Vector2F &positon)
	{
		m_position = m_prevPosition = positon; 
	}

	Vector2I getPosition() const;
	Vector2F getDrawPosition(const float alpha) const;
	int getX() const { return getPosition().x; }
	int getY() const { return getPosition().y; }

	// Size
	void setSize(Vector2I size);
	void setSize(const uint width, const uint height) { setSize(Vector2I(width, height)); }
	Vector2I getSize() const;
	uint getWidth() const { return getSize().x; }
	uint getHeight() const { return getSize().y; }

	// Get input position
	Vector2I getInputPosition() const;

	// Update
	void onTick(TickEvent *e);

	// Interpolate
	void onDraw(DrawEvent *e);

	// MouseListener event
	void onMouseWheel(MouseEvent *e);

	// WindowListener event
	void onWindowSizeChanged(WindowEvent *e);

private:
	InputManager *m_input;
	Window *m_window;

	// Position
	Vector2F m_position, m_prevPosition, m_velocity;

	// Size
	Vector2I m_size;

	// Zoom
	float m_zoomLevel;

	// Target entity
	Entity *m_tagetEntity;
};

#endif // CAMERA_H