#include "camera.h"
#include "game.h"

Camera::Camera() :
	GameObject(DRAW_ORDER_CAMERA),
	m_position(0.0f, 0.0f),
	m_zoom(1.0f)
{
	xd::Input::addMouseListener(this);
}

Vector2 Camera::getCenter() const
{
	return m_position + xd::Window::getSize() * 0.5f / m_zoom;
}

Matrix4 Camera::getProjectionMatrix() const
{
	Matrix4 mat;
	mat.translate(-m_position.x, -m_position.y, 0.0f);
	mat.scale(m_zoom);
	return mat;
}

void Camera::lookAt(Vector2 point)
{
	point -= xd::Window::getSize() * 0.5f / m_zoom;
	m_position = point;
}

Vector2 Camera::getPosition()
{
	return m_position;
}

uint Camera::getX()
{
	return m_position.x;
}

uint Camera::getY()
{
	return m_position.y;
}

Vector2i Camera::getSize()
{
	return xd::Window::getSize();
}

uint Camera::getWidth()
{
	return getSize().x;
}

uint Camera::getHeight()
{
	return getSize().y;
}

Vector2i Camera::getSizeZoomed()
{
	return xd::Window::getSize() / m_zoom;
}

uint Camera::getWidthZoomed()
{
	return getSizeZoomed().x;
}

uint Camera::getHeightZoomed()
{
	return getSizeZoomed().y;
}

float Camera::getZoom() const
{
	return m_zoom;
}

void Camera::update()
{
	if(xd::Input::getKeyState(xd::XD_KEY_LEFT)) {
		m_position.x -= 16.0f/m_zoom;
	}
	
	if(xd::Input::getKeyState(xd::XD_KEY_RIGHT)) {
		m_position.x += 16.0f/m_zoom;
	}
	
	if(xd::Input::getKeyState(xd::XD_KEY_UP)) {
		m_position.y -= 16.0f/m_zoom;
	}
	
	if(xd::Input::getKeyState(xd::XD_KEY_DOWN)) {
		m_position.y += 16.0f/m_zoom;
	}
	
	if(xd::Input::getKeyState(xd::XD_KEY_PLUS)) {
		Vector2 center = getCenter();
		m_zoom *= 2.0f;
		lookAt(center);
	}

	if(xd::Input::getKeyState(xd::XD_KEY_MINUS)) {
		Vector2 center = getCenter();
		m_zoom *= 0.5f;
		lookAt(center);
	}
}

void Camera::draw(xd::SpriteBatch*)
{
	Vector2 center = getCenter();
	World::getDebug()->setVariable("Camera", xd::util::floatToStr(center.x) + ", " + xd::util::floatToStr(center.y));
	World::getDebug()->setVariable("Zoom", xd::util::floatToStr(m_zoom));
}

void Camera::mouseWheelEvent(const int dt)
{
	Vector2 center = getCenter();
	if(dt < 0) m_zoom *= 0.5f;
	if(dt > 0) m_zoom *= 2.0f;
	lookAt(center);
}