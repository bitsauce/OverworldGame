#include "UiObject.h"

UiObject::UiObject(UiObject *parent) :
	m_parent(parent),
	m_anchor(0.0f, 0.0f),
	m_origin(0.0f, 0.0f),
	m_rect(),
	m_hovered(false),
	m_pressed(false),
	m_focused(false),
	m_clickCount(0),
	m_clickTimer()
{
	if(parent)
	{
		parent->addChildLast(this);
	}
}

UiObject::~UiObject()
{
}

bool UiObject::isFocused() const
{
	return m_focused;
}

void UiObject::setFocused(const bool focused)
{
	m_focused = focused;
}

bool UiObject::isHovered() const
{
	return m_hovered;
}

bool UiObject::isPressed() const
{
	return m_pressed;
}

void UiObject::setPosition(const Vector2F &position)
{
	setPosition(position.x, position.y);
}

void UiObject::setPosition(const float x, const float y)
{
	m_rect.position.set(x, y);
}

Vector2F UiObject::getPosition() const
{
	return m_rect.position;
}

void UiObject::setOrigin(const Vector2F &origin)
{
	m_origin = origin;
}

void UiObject::setOrigin(const float x, const float y)
{
	m_origin.set(x, y);
}

Vector2F UiObject::getOrigin() const
{
	return m_origin;
}

void UiObject::setSize(const Vector2F &size)
{
	setSize(size.x, size.y);
}

void UiObject::setSize(const float width, const float height)
{
	m_rect.size.set(width, height);

	Vector2I size = getDrawSize();
	ResizeEvent e(size.x, size.y);
	onResize(&e);
}

Vector2F UiObject::getSize() const
{
	return m_rect.size;
}

void UiObject::setRect(const RectF &rect)
{
	m_rect = rect;
}

RectF UiObject::getRect() const
{
	return m_rect;
}

void UiObject::setAnchor(const Vector2F &anchor)
{
	setAnchor(anchor.x, anchor.y);
}

Vector2F UiObject::getAnchor() const
{
	return m_anchor;
}

void UiObject::setAnchor(const float x, const float y)
{
	m_anchor.set(x, y);
}

Vector2I UiObject::getDrawPosition()
{
	if(!m_parent)
	{
		LOG("UiObject not a child of Canvas!");
		return Vector2I();
	}

	Vector2F parentPos = m_parent->getDrawPosition();
	Vector2F parentSize = m_parent->getDrawSize();
	Vector2F pos = m_rect.position;
	Vector2F size = m_rect.size;

	parentPos += parentSize * m_anchor;
	pos -= size * m_origin;
	return parentPos + pos * parentSize;
}

Vector2I UiObject::getDrawSize()
{
	if(!m_parent)
	{
		LOG("UiObject not a child of Canvas!");
		return Vector2I();
	}
	return m_rect.size * m_parent->getDrawSize();
}

RectI UiObject::getDrawRect()
{
	return RectI(getDrawPosition(), getDrawSize());
}

void UiObject::onDraw(DrawEvent *e)
{
	RectI rect = getDrawRect();

	/*GraphicsContext *g = e->getGraphicsContext();
	g->drawRectangle(rect, Color(20, 20, 255, 127));

	g->drawCircle(rect.position + rect.size * m_origin, 2.5f, 5, Color(20, 255, 20, 255));

	if(m_parent)
	{
		RectI parentRect = m_parent->getDrawRect();
		g->drawCircle(parentRect.position + parentRect.size * m_anchor, 2.5f, 5, Color(255, 20, 20, 255));
	}*/

	SceneObject::onDraw(e);
}

void UiObject::onMouseEvent(MouseEvent *e)
{
	switch(e->getType())
	{
		case MouseEvent::MOVE:
		{
			if(getDrawRect().contains(e->getPosition()))
			{
				m_hovered = true;
				HoverEvent event(HoverEvent::ENTER, e);
				onHover(&event);
			}
			else
			{
				m_hovered = false;
				HoverEvent event(HoverEvent::LEAVE, e);
				onHover(&event);
			}

			if(m_pressed)
			{
				ClickEvent event(ClickEvent::DRAG, m_clickCount, e);
				onClick(&event);
			}
		}
		break;

		case MouseEvent::DOWN:
		{
			if(m_hovered)
			{
				m_pressed = true;

				if(m_clickTimer.stop() < 0.2f)
				{
					m_clickTimer.start();
				}
				else
				{
					m_clickCount = 0;
				}

				m_clickCount++;

				ClickEvent event(ClickEvent::BEGIN, m_clickCount, e);
				onClick(&event);
			}
		}
		break;

		case MouseEvent::UP:
		{
			if(m_pressed)
			{
				if(m_hovered)
				{
					ClickEvent event(ClickEvent::DONE, m_clickCount, e);
					onClick(&event);

					if(m_clickTimer.stop() < 0.2f)
					{
						m_clickTimer.start();
					}
					else
					{
						m_clickCount = 0;
					}

					if(!m_focused)
					{
						m_focused = true;
						FocusEvent event(FocusEvent::GAINED);
						onFocus(&event);
					}
				}
				else
				{
					ClickEvent event(ClickEvent::CANCELED, m_clickCount, e);
					onClick(&event);
				}
				m_pressed = false;
			}
			else if(!m_hovered && m_focused)
			{
				m_focused = false;
				FocusEvent event(FocusEvent::LOST);
				onFocus(&event);
			}
		}
		break;
	}
	SceneObject::onMouseEvent(e);
}