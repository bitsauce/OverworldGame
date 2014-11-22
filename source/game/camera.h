#ifndef CAMERA_H
#define CAMERA_H

#include <x2d/x2d.h>

class Camera
{
public:
	static Vector2 getCenter();
	static void lookAt(Vector2 point);
	static Matrix4 getProjectionMatrix();

	static Vector2 getPosition();
	static float getX();
	static float getY();

private:
	static Vector2 s_position;
	static float s_zoom;
};

#endif // CAMERA_H