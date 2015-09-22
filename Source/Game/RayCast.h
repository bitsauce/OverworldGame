#ifndef RAY_CAST_H
#define RAY_CAST_H

#include "Config.h"

class RayCast
{
public:
	RayCast(function<bool(int, int)> plotTest);

	bool trace(const Vector2i p0, const Vector2i p1);

	vector<Vector2i> getPoints() const { return m_points; }

private:
	// Plot
	bool plot(const int x, const int y);

	// The rasterized points
	vector<Vector2i> m_points;
	
	// A func call for plot testing
	function<bool(int, int)> m_plotTest;
	
	// The raycast range
	float m_range;
};

#endif // RAY_CAST_H