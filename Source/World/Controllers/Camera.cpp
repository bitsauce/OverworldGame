#include "Camera.h"
#include "Constants.h"
#include "World/World.h"
#include "Game/Debug.h"
#include "Entities/Pawn.h"

Camera::Camera(World *world, InputManager *input, Window *window) :
	Entity(world, ENTITY_CAMERA),
	m_position(0.0f, 0.0f),
	m_prevPosition(0.0f, 0.0f),
	m_velocity(0.0f, 0.0f),
	m_tagetEntity(0),
	m_input(input),
	m_window(window)
{
	setZoomLevel(1.0f);
}

Vector2i Camera::getCenter(const float alpha) const
{
	return math::lerp(m_prevPosition, m_position, alpha) + m_size * 0.5f;
}

Matrix4 Camera::getTransformationMatrix(const float alpha) const
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

Vector2i Camera::getInputPosition() const
{
	return m_position + m_input->getPosition() / m_zoomLevel;
}

Vector2i Camera::getSize() const
{
	return m_size;
}

void Camera::setZoomLevel(const float zoomLevel)
{
	Vector2 center = getCenter(0.0f);
	m_zoomLevel = zoomLevel;
	m_size = m_window->getSize() / m_zoomLevel;
	lookAt(center);
}

float Camera::getZoomLevel() const
{
	return m_zoomLevel;
}

void Camera::onTick(TickEvent *e)
{
	if(!m_tagetEntity)
	{
		float acc = (m_input->getKeyState(CGF_KEY_LCTRL) ? 32.0f : 256.0f) * e->getDelta();

		m_prevPosition = m_position;

		if(m_input->getKeyState(CGF_KEY_LEFT))
		{
			m_velocity.x -= acc;
		}

		if(m_input->getKeyState(CGF_KEY_RIGHT))
		{
			m_velocity.x += acc;
		}

		if(m_input->getKeyState(CGF_KEY_UP))
		{
			m_velocity.y -= acc;
		}

		if(m_input->getKeyState(CGF_KEY_DOWN))
		{
			m_velocity.y += acc;
		}

		m_velocity *= 0.75f;
		if(m_velocity.magnitude() > 64.0f)
		{
			m_velocity = m_velocity.normalized() * 64.0f;
		}

		m_position += m_velocity;
	}
}

void Camera::onDraw(DrawEvent *e)
{
	if(m_tagetEntity)
	{
		lookAt(m_tagetEntity->getDrawPosition(e->getAlpha()) + m_tagetEntity->getSize() * 0.5f);
		m_prevPosition = m_position;
	}
}

void Camera::onMouseWheel(MouseEvent *e)
{
	/*Vector2 center = getCenter();
	if(delta > 0) zoomIn();
	if(delta < 0) zoomOut();
	lookAt(center);*/
}

void Camera::onWindowSizeChanged(WindowEvent *e)
{
	setZoomLevel(m_zoomLevel);
}