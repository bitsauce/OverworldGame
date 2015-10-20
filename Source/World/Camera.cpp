#include "Camera.h"
#include "Constants.h"
#include "World/World.h"
#include "Game/Debug.h"
#include "Entities/Entity.h"
#include "Entities/Dynamic/Pawn.h"

Camera::Camera() :
	m_position(0.0f, 0.0f),
	m_prevPosition(0.0f, 0.0f),
	m_velocity(0.0f, 0.0f),
	m_tagetEntity(0)
{
	setZoomLevel(1.0f);
}

Vector2 Camera::getCenter(const float alpha) const
{
	return math::lerp(m_prevPosition, m_position, alpha) + m_size * 0.5f;
}

Matrix4 Camera::getModelViewMatrix(const float alpha) const
{
	Vector2 pos = math::lerp(m_prevPosition, m_position, alpha);

	Matrix4 mat;
	mat.translate(-pos.x, -pos.y, 0.0f);
	mat.scale(m_zoomLevel);
	return mat;
}

void Camera::lookAt(Vector2 worldPoint)
{
	worldPoint -= m_size * 0.5f;
	m_position = worldPoint;
}

void Camera::setTargetEntity(DynamicEntity * target)
{
	m_tagetEntity = target;
}

Vector2i Camera::getPosition() const
{
	return m_position;
}

Vector2 Camera::getInputPosition() const
{
	return m_position + Input::getPosition() / m_zoomLevel;
}

Vector2i Camera::getSize() const
{
	return m_size;
}

void Camera::setZoomLevel(const float zoomLevel)
{
	Vector2 center = getCenter(0.0f);
	m_zoomLevel = zoomLevel;
	m_size = Window::getSize() / m_zoomLevel;
	lookAt(center);
}

float Camera::getZoomLevel() const
{
	return m_zoomLevel;
}

void Camera::update(const float dt)
{

	if (!m_tagetEntity)
	{
		float acc = (Input::getKeyState(XD_KEY_LEFT_CONTROL) == GLFW_PRESS ? 32.0f : 256.0f) * dt;

		m_prevPosition = m_position;

		if (Input::getKeyState(XD_KEY_LEFT) == GLFW_PRESS) {
			m_velocity.x -= acc;
		}
	
		if(Input::getKeyState(XD_KEY_RIGHT) == GLFW_PRESS) {
			m_velocity.x += acc;
		}
	
		if(Input::getKeyState(XD_KEY_UP) == GLFW_PRESS) {
			m_velocity.y -= acc;
		}
	
		if(Input::getKeyState(XD_KEY_DOWN) == GLFW_PRESS) {
			m_velocity.y += acc;
		}

		m_velocity *= 0.75f;
		if (m_velocity.magnitude() > 64.0f)
		{
			m_velocity = m_velocity.normalized() * 64.0f;
		}

		m_position += m_velocity;
	}
}

void Camera::interpolate(const float alpha)
{
	if (m_tagetEntity)
	{
		lookAt(m_tagetEntity->getDrawPosition(alpha) + m_tagetEntity->getSize() * 0.5f);
		m_prevPosition = m_position;
	}
}

void Camera::mouseWheelEvent(const int delta)
{
	/*Vector2 center = getCenter();
	if(delta > 0) zoomIn();
	if(delta < 0) zoomOut();
	lookAt(center);*/
}

void Camera::resizeEvent(uint width, uint height)
{
	setZoomLevel(m_zoomLevel);
}