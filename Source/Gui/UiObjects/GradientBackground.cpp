#include "GradientBackground.h"

GradientBackground::GradientBackground(UiObject *parent) :
	UiObject(parent),
	m_topColor(200, 200, 200, 255),
	m_bottomColor(250, 250, 250, 255)
{
	setSize(1.0f, 1.0f);
}

void GradientBackground::onDraw(DrawEvent *e)
{
	RectI rect = { Vector2I(0), e->getGraphicsContext()->getSize() };

	// Draw gradient
	m_vertices[0].set2f(VERTEX_POSITION, rect.getLeft(), rect.getTop());
	m_vertices[1].set2f(VERTEX_POSITION, rect.getLeft(), rect.getBottom());
	m_vertices[2].set2f(VERTEX_POSITION, rect.getRight(), rect.getTop());
	m_vertices[3].set2f(VERTEX_POSITION, rect.getRight(), rect.getBottom());
	m_vertices[0].set4ub(VERTEX_COLOR, m_topColor.getR(), m_topColor.getG(), m_topColor.getB(), m_topColor.getA());
	m_vertices[1].set4ub(VERTEX_COLOR, m_bottomColor.getR(), m_bottomColor.getG(), m_bottomColor.getB(), m_bottomColor.getA());
	m_vertices[2].set4ub(VERTEX_COLOR, m_topColor.getR(), m_topColor.getG(), m_topColor.getB(), m_topColor.getA());
	m_vertices[3].set4ub(VERTEX_COLOR, m_bottomColor.getR(), m_bottomColor.getG(), m_bottomColor.getB(), m_bottomColor.getA());
	e->getGraphicsContext()->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, m_vertices, 4);

	UiObject::onDraw(e);
}
