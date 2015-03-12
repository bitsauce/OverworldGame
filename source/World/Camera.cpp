#include "Camera.h"
#include "Constants.h"
#include "World/World.h"
#include "World/Debug.h"
#include "Entities/Entity.h"

Camera::Camera() :
	m_position(0.0f, 0.0f)
{
	Input::bind(XD_KEY_PLUS, function<void()>(bind(&Camera::zoomIn, this)));
	Input::bind(XD_KEY_MINUS, function<void()>(bind(&Camera::zoomOut, this)));

	setZoomLevel(1.0f);
}

Vector2 Camera::getCenter() const
{
	return m_position + m_size * 0.5f;
}

Matrix4 Camera::getProjectionMatrix() const
{
	Matrix4 mat;
	mat.translate(-m_position.x, -m_position.y, 0.0f);
	mat.scale(m_zoomLevel);
	return mat;
}

void Camera::lookAt(Vector2 worldPoint)
{
	worldPoint -= m_size * 0.5f;
	m_position = worldPoint;
}

Vector2i Camera::getPosition() const
{
	return m_position;
}

Vector2 Camera::getInputPosition() const
{
	return m_position + Input::getPosition();
}

Vector2i Camera::getSize() const
{
	return m_size;
}

void Camera::setZoomLevel(const float zoomLevel)
{
	Vector2 center = getCenter();
	m_zoomLevel = zoomLevel;
	m_size = Window::getSize() / m_zoomLevel;
	lookAt(center);
}

float Camera::getZoomLevel() const
{
	return m_zoomLevel;
}

#include "Entities/Player.h"

void Camera::update(const float alpha)
{
	if(m_tagetEntity)
	{
		PhysicsBody *body = &((Player*)m_tagetEntity)->getBody();
		lookAt(body->getDrawPosition(alpha) + body->getSize() * 0.5f);
	}
	else
	{
		float speed = Input::getKeyState(XD_KEY_LCONTROL) ? 1.0f : 16.0f;
		if(Input::getKeyState(XD_KEY_LEFT)) {
			m_position.x -= speed/m_zoomLevel;
		}
	
		if(Input::getKeyState(XD_KEY_RIGHT)) {
			m_position.x += speed/m_zoomLevel;
		}
	
		if(Input::getKeyState(XD_KEY_UP)) {
			m_position.y -= speed/m_zoomLevel;
		}
	
		if(Input::getKeyState(XD_KEY_DOWN)) {
			m_position.y += speed/m_zoomLevel;
		}
	}
}

void Camera::mouseWheelEvent(const int dt)
{
	/*Vector2 center = getCenter();
	if(dt > 0) zoomIn();
	if(dt < 0) zoomOut();
	lookAt(center);*/
}

void Camera::resizeEvent(uint width, uint height)
{
	setZoomLevel(m_zoomLevel);
}