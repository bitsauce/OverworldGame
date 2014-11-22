#include "camera.h"

Vector2 Camera::s_position(0.0f, 0.0f);
float Camera::s_zoom = 0.0f;

Vector2 Camera::getCenter()
{
	return s_position + XWindow::getSize()*0.5f;
}

void Camera::lookAt(Vector2 point)
{
	Vector2 windowSize = XWindow::getSize();
	point -= windowSize/(2.0f*s_zoom);
	s_position = point;
}

Matrix4 Camera::getProjectionMatrix()
{
	Matrix4 mat;
	mat.translate(-s_position.x, -s_position.y, 0.0f);
	mat.scale(s_zoom);
	return mat;
}

Vector2 Camera::getPosition()
{
	return s_position;
}

float Camera::getX()
{
	return s_position.x;
}

float Camera::getY()
{
	return s_position.y;
}