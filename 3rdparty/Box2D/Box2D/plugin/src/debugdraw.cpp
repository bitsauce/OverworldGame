#include "debugdraw.h"
#include "box2d.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_LINES);
	batch->setTexture(0);

	XVertex *data = new XVertex[vertexCount];
	vector<uint> indices;
	for(int i = 0; i < vertexCount; i++)
	{
		Vector2 pos = toXDVec(vertices[i]);
		data[i].set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));
		data[i].set4f(VERTEX_POSITION, pos.x, pos.y);

		if(i > 0)
		{
			indices.push_back(i-1);
			indices.push_back(i);
		}
	}

	indices.push_back(vertexCount-1);
	indices.push_back(0);

	batch->addVertices(data, vertexCount, indices.data(), indices.size());

	delete[] data;
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
	batch->setTexture(0);
	
	XVertex *data = new XVertex[vertexCount];
	vector<uint> indices;

	for(int i = 0; i < vertexCount; i++)
	{
		Vector2 pos = toXDVec(vertices[i]);
		data[i].set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));
		data[i].set4f(VERTEX_POSITION, pos.x, pos.y);
		
		if(i >= 2)
		{
			indices.push_back(0);
			indices.push_back(i < vertexCount ? i : 1);
			indices.push_back(i-1);
		}
	}
	
	batch->addVertices(data, vertexCount, indices.data(), indices.size());

	delete[] data;

	DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const int segments = 16;
	const float increment = 2.0f * b2_pi / segments;
	float theta = 0.0f;

	b2Vec2 *vertices = new b2Vec2[segments+2];

	vertices[0] = center;
	for(int i = 1; i < segments+2; i++)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vertices[i] = v;
		theta += increment;
	}

	DrawPolygon(vertices, segments+2, color);

	delete[] vertices;
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const int segments = 16;
	const float increment = 2.0f * b2_pi / segments;
	float theta = atan2(axis.y, axis.x);

	b2Vec2 *vertices = new b2Vec2[segments+2];

	vertices[0] = center;
	for(int i = 1; i < segments+2; i++)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vertices[i] = v;
		theta += increment;
	}

	DrawSolidPolygon(vertices, segments+2, color);
	DrawPolygon(vertices, segments+2, color);

	delete[] vertices;
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	b2Vec2 *vertices = new b2Vec2[2];
	vertices[0] = p1;
	vertices[1] = p2;
	DrawPolygon(vertices, 2, color);
	delete[] vertices;
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_LINES);
	batch->setTexture(0);

	b2Vec2 p1 = xf.p, p2;
	const float32 axisScale = 0.4f;

	XVertex *data = new XVertex[4];
	
	Vector2 pos = toXDVec(p1);
	data[0].set4f(VERTEX_POSITION, pos.x, pos.y);
	data[0].set4ub(VERTEX_COLOR, 255, 0, 0, 255);

	p2 = p1 + axisScale * xf.q.GetXAxis();
	
	pos = toXDVec(p2);
	data[1].set4f(VERTEX_POSITION, pos.x, pos.y);
	data[1].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
	
	pos = toXDVec(p1);
	data[2].set4f(VERTEX_POSITION, pos.x, pos.y);
	data[2].set4ub(VERTEX_COLOR, 0, 255, 0, 255);

	p2 = p1 + axisScale * xf.q.GetYAxis();
	
	pos = toXDVec(p2);
	data[3].set4f(VERTEX_POSITION, pos.x, pos.y);
	data[3].set4ub(VERTEX_COLOR, 0, 255, 0, 255);

	uint indices[4] = { 0, 1, 2, 3 };

	batch->addVertices(data, 4, indices, 4);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_POINTS);
	batch->setTexture(0);

	XVertex vertex;
	Vector2 pos = toXDVec(p);
	vertex.set4f(VERTEX_POSITION, pos.x, pos.y);
	vertex.set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));
	
	uint indices = 0;

	batch->addVertices(&vertex, 1, &indices, 1);
}

void DebugDraw::DrawString(int x, int y, const char* string, ...)
{
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);

	XVertex *data = new XVertex[4];
	Vector2 upper = toXDVec(aabb->upperBound);
	Vector2 lower = toXDVec(aabb->lowerBound);
	
	data[0].set4f(VERTEX_POSITION, lower.x, lower.y);
	data[0].set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));
	
	data[1].set4f(VERTEX_POSITION, upper.x, lower.y);
	data[1].set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));
	
	data[2].set4f(VERTEX_POSITION, upper.x, upper.y);
	data[2].set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));

	data[3].set4f(VERTEX_POSITION, lower.x, upper.y);
	data[3].set4ub(VERTEX_COLOR, uchar(color.r * 255), uchar(color.g * 255), uchar(color.b * 255), uchar(255));

	batch->addVertices(data, 4, QUAD_INDICES, 6);

	delete[] data;
}