//  Algorithm from: http://en.wikipedia.org/wiki/Bresenham's_line_algorithm

#include "rayCast.h"

RayCast::RayCast(function<bool(int, int)> plotTest) :
	m_plotTest(plotTest)
{
}

// This function casts a ray from p0 to p1
bool RayCast::trace(const Vector2I p0, const Vector2I p1)
{
	// Clear previous points
	m_points.clear();
		
	// Get integer points
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;
		
	// Get deltas
	int dx = abs(x1-x0);
	int dy = abs(y1-y0);
		
	// Get line dir
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
		
	// Get ???
	int a = dx-dy;
		
	// Perform line plotting
	bool col = false;
	while(true)
	{
		// Plot the current pos
		if(!plot(x0, y0))
		{
			// Plot test failed, break
			col = true;
			break;
		}
			
		// Check if we have reached the end
		if((x0 == x1 && y0 == y1) || (m_range > 0.0f && (Vector2F(x0, y0) - Vector2F(p0)).magnitude() > m_range))
		{
			break;
		}	
			
		// Apply y traversal
		int a2 = a*2;
		if(a2 > -dy)
		{
			a -= dy;
			x0 += sx;
		}
			
		// Apply x traversal
		if(a2 < dx)
		{
			a += dx;
			y0 += sy;
		}
	}
		
	// Return result
	return col;
}
	
bool RayCast::plot(const int x, const int y)
{
	// Plot the point
	m_points.push_back(Vector2I(x, y));
	return m_plotTest == nullptr || m_plotTest(x, y);
}