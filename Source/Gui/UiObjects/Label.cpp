#include "Label.h"

Label::Label(UiObject *parent) :
	UiObject(parent),
	m_font(Resource<Font>("Font")),
	m_text(""),
	m_spriteBatch()
{
}

void Label::onDraw(DrawEvent *e)
{
	RectF rect = getDrawRect();
	GraphicsContext *g = e->getGraphicsContext();

	m_font->setColor(Color(0, 0, 0, 255));

	m_spriteBatch.begin(g);
	m_font->setHeight(min(rect.size.y, 16.0f));
	m_font->setColor(Color(0, 0, 0, 255));
	m_font->drawBox(&m_spriteBatch, rect.position.x, rect.position.y - m_font->getHeight() * 0.5f, rect.size.x, m_text);
	m_font->setHeight(16.0f);
	m_spriteBatch.end();

	UiObject::onDraw(e);
}