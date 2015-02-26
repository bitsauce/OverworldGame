#ifndef RAY_CAST_H
#define RAY_CAST_H

#include "Config.h"

class RayCast
{
public:

	bool test(const Vector2i p0, const Vector2i p1);
	bool plot(const int x, const int y);

	vector<Vector2i> getPoints() const { return m_points; }

private:
	// The rasterized points
	vector<Vector2i> m_points;
	
	// A func call for plot testing
	function<bool(int, int)> m_plotTest;
	
	// The raycast range
	float m_range;
};

#endif // RAY_CAST_H