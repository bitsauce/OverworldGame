#include "Camera.h"
#include "Constants.h"
#include "World/World.h"
#include "Game/Debug.h"
#include "Entities/Pawn.h"

Camera::Camera(World *world, InputManager *input, Window *window) :
	m_position(0.0f, 0.0f),
	m_prevPosition(0.0f, 0.0f),
	m_velocity(0.0f, 0.0f),
	m_tagetEntity(0),
	m_input(input),
	m_window(window)
{
	setZoomLevel(1.0f);
}

Vector2I Camera::getCenter(const float alpha) const
{
	return math::lerp(m_prevPosition, m_position, alpha) + m_size * 0.5f;
}


#define AVOID_DELTA 0.01f

float avoidZeroPointFive(const float f)
{
	float base;
	if(f >= 0.0f)
	{
		float fract = modf(f, &base);
		if(fract > (0.5f - AVOID_DELTA) && fract <= 0.5f)
		{
			base += (0.5f - AVOID_DELTA);
		}
		else if(fract < (0.5f + AVOID_DELTA) && fract >= 0.5f)
		{
			base += (0.5f + AVOID_DELTA);
		}
		else
		{
			base += fract;
		}
	}
	else
	{
		float fract = -modf(f, &base);
		if(fract > (0.5f - AVOID_DELTA) && fract <= 0.5f)
		{
			base -= (0.5f - AVOID_DELTA);
		}
		else if(fract < (0.5f + AVOID_DELTA) && fract >= 0.5f)
		{
			base -= (0.5f + AVOID_DELTA);
		}
		else
		{
			base -= fract;
		}
	}
	return base;
}

Vector2F avoidZeroPointFive(const Vector2F &pos)
{
	Vector2F out;
	out.x = avoidZeroPointFive(pos.x);
	out.y = avoidZeroPointFive(pos.y);
	return out;
}

Matrix4 Camera::getTransformationMatrix(const float alpha) const
{
	Vector2F pos = avoidZeroPointFive(math::lerp(m_prevPosition, m_position, alpha));

	Matrix4 mat;
	mat.translate(-pos.x, -pos.y, 0.0f);
	mat.scale(m_zoomLevel);
	return mat;
}

void Camera::lookAt(Vector2F worldPoint)
{
	worldPoint -= m_size * 0.5f;
	m_position = worldPoint;
}

void Camera::setTargetEntity(Entity * target)
{
	m_tagetEntity = target;
}

Vector2I Camera::getPosition() const
{
	return m_position;
}

Vector2F Camera::getDrawPosition(const float alpha) const
{
	return avoidZeroPointFive(math::lerp(m_prevPosition, m_position, alpha));
}

Vector2I Camera::getInputPosition() const
{
	return m_position + m_input->getPosition() / m_zoomLevel;
}

Vector2I Camera::getSize() const
{
	return m_size;
}

void Camera::setZoomLevel(const float zoomLevel)
{
	Vector2F center = getCenter(0.0f);
	m_zoomLevel = zoomLevel;
	m_size = m_window->getSize() / m_zoomLevel;
	lookAt(center);
	m_prevPosition = m_position;
}

float Camera::getZoomLevel() const
{
	return m_zoomLevel;
}

void Camera::zoomIn(InputEvent *e)
{
	if(e->getType() == KeyEvent::DOWN)
	{
		setZoomLevel(m_zoomLevel * 2.0f);
	}
}

void Camera::zoomOut(InputEvent *e)
{
	if(e->getType() == KeyEvent::DOWN)
	{
		setZoomLevel(m_zoomLevel * 0.5f);
	}
}

void Camera::onTick(TickEvent *e)
{
	if(!m_tagetEntity)
	{
		float acc = (m_input->getKeyState(SAUCE_KEY_LSHIFT) ? 256.0f : 512.0f) * e->getDelta();

		m_prevPosition = m_position;

		if(m_input->getKeyState(SAUCE_KEY_LEFT))
		{
			m_velocity.x -= acc;
		}

		if(m_input->getKeyState(SAUCE_KEY_RIGHT))
		{
			m_velocity.x += acc;
		}

		if(m_input->getKeyState(SAUCE_KEY_UP))
		{
			m_velocity.y -= acc;
		}

		if(m_input->getKeyState(SAUCE_KEY_DOWN))
		{
			m_velocity.y += acc;
		}

		m_velocity *= 0.75f;
		if(m_velocity.length() > 64.0f)
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
	/*Vector2F center = getCenter();
	if(delta > 0) zoomIn();
	if(delta < 0) zoomOut();
	lookAt(center);*/
}

void Camera::onWindowSizeChanged(WindowEvent *e)
{
	setZoomLevel(m_zoomLevel);
}