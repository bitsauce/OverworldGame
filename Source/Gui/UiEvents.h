#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#include "Config.h"

enum
{
	EVENT_TYPE_CLICK_BEGIN = EVENT_CUSTOM,
	EVENT_TYPE_CLICK_DRAG,
	EVENT_TYPE_CLICK_DONE,
	EVENT_TYPE_CLICK_CANCELED,
	EVENT_TYPE_HOVER_ENTER,
	EVENT_TYPE_HOVER_LEAVE,
	EVENT_TYPE_FOCUS_GAINED,
	EVENT_TYPE_FOCUS_LOST,
	EVENT_TYPE_RESIZE
};

class ClickEvent : public Event
{
public:
	enum ClickEventType
	{
		BEGIN = EVENT_TYPE_CLICK_BEGIN,
		DRAG = EVENT_TYPE_CLICK_DRAG,
		DONE = EVENT_TYPE_CLICK_DONE,
		CANCELED = EVENT_TYPE_CLICK_CANCELED
	};

	ClickEvent(ClickEventType type, int clickCount, MouseEvent *mouseEvent) :
		Event(type),
		m_clickCount(clickCount),
		m_mouseEvent(mouseEvent)
	{
	}

	int getClickCount() const
	{
		return m_clickCount;
	}

	MouseEvent *getMouseEvent() const
	{
		return m_mouseEvent;
	}

private:
	const int m_clickCount;
	MouseEvent *const m_mouseEvent;
};

class HoverEvent : public Event
{
public:
	enum HoverEventType
	{
		ENTER = EVENT_TYPE_HOVER_ENTER,
		LEAVE = EVENT_TYPE_HOVER_LEAVE
	};

	HoverEvent(HoverEventType type, MouseEvent *mouseEvent) :
		Event(type),
		m_mouseEvent(mouseEvent)
	{
	}

	MouseEvent *getMouseEvent() const
	{
		return m_mouseEvent;
	}

private:
	MouseEvent *m_mouseEvent;
};

class FocusEvent : public Event
{
public:
	enum FocusEventType
	{
		GAINED = EVENT_TYPE_FOCUS_GAINED,
		LOST = EVENT_TYPE_FOCUS_LOST
	};

	FocusEvent(FocusEventType type) :
		Event(type)
	{
	}

private:

};

class ResizeEvent : public Event
{
public:

	ResizeEvent(const uint width, const uint height) :
		Event(EVENT_TYPE_RESIZE),
		m_width(width),
		m_height(height)
	{
	}

	uint getWidth() const
	{
		return m_width;
	}

	uint getHeight() const
	{
		return m_height;
	}

private:
	const uint m_width, m_height;
};

#endif // UI_EVENTS_H