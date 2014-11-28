#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <x2d/x2d.h>

class Simplex2D
{
public:
	Simplex2D();
	
	float valueAt(float x, float y);
	float dot(const int *g, const float x, const float y);

	int m_octaves;
	float m_gain;
	float m_frequency;

private:
	float rawNoise(float x, float y);
};

class Simplex3D
{
public:
	Simplex3D();
	
	float valueAt(const float x, const float y, const float z);
	float dot(const int *g, const float x, const float y, const float z);

	int m_octaves;
	float m_gain;
	float m_frequency;
	
private:
	float rawNoise(const float x, const float y, const float z);
};

#endif // SIMPLEX_H