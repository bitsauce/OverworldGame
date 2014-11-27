#include "camera.h"
#include "debug.h"
#include "constants.h"

Camera::Camera() :
	GameObject(DRAW_ORDER_CAMERA),
	m_position(0.0f, 0.0f),
	m_zoom(1.0f)
{
	XInput::addMouseListener(this);
}

Vector2 Camera::getCenter()
{
	return m_position + XWindow::getSize() * 0.5f / m_zoom;
}

void Camera::lookAt(Vector2 point)
{
	point -= XWindow::getSize() * 0.5f / m_zoom;
	m_position = point;
}

Matrix4 Camera::getProjectionMatrix()
{
	Matrix4 mat;
	mat.translate(-m_position.x, -m_position.y, 0.0f);
	mat.scale(m_zoom);
	return mat;
}

Vector2 Camera::getPosition()
{
	return m_position;
}

float Camera::getX()
{
	return m_position.x;
}

float Camera::getY()
{
	return m_position.y;
}

Vector2 Camera::getSize()
{
	return XWindow::getSize() / m_zoom;
}

float Camera::getWidth()
{
	return getSize().x;
}

float Camera::getHeight()
{
	return getSize().y;
}

void Camera::update()
{
	if(XInput::getKeyState(XD_KEY_LEFT)) {
		m_position.x -= 16.0f/m_zoom;
	}
	if(XInput::getKeyState(XD_KEY_RIGHT)) {
		m_position.x += 16.0f/m_zoom;
	}
	if(XInput::getKeyState(XD_KEY_UP)) {
		m_position.y -= 16.0f/m_zoom;
	}
	if(XInput::getKeyState(XD_KEY_DOWN)) {
		m_position.y += 16.0f/m_zoom;
	}
}

void Camera::draw(XBatch *batch)
{
	Vector2 center = getCenter();
	Debug::setVariable("Camera", util::floatToStr(center.x) + ", " + util::floatToStr(center.y));
	Debug::setVariable("Zoom", util::floatToStr(m_zoom));
}

void Camera::mouseWheelEvent(const int dt)
{
	Vector2 center = getCenter();
	if(dt < 0) m_zoom *= 0.5f;
	if(dt > 0) m_zoom *= 2.0f;
	lookAt(center);
}