#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#include "Config.h"

enum
{
	EVENT_GUI_CLICK_BEGIN = EVENT_CUSTOM,
	EVENT_GUI_CLICK_DRAG,
	EVENT_GUI_CLICK_DONE,
	EVENT_GUI_CLICK_CANCELED,
	EVENT_GUI_HOVER_ENTER,
	EVENT_GUI_HOVER_LEAVE,
	EVENT_GUI_FOCUS_GAINED,
	EVENT_GUI_FOCUS_LOST,
	EVENT_GUI_RESIZE
};

class ClickEvent : public Event
{
public:
	enum ClickEventType
	{
		BEGIN = EVENT_GUI_CLICK_BEGIN,
		DRAG = EVENT_GUI_CLICK_DRAG,
		DONE = EVENT_GUI_CLICK_DONE,
		CANCELED = EVENT_GUI_CLICK_CANCELED
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
		ENTER = EVENT_GUI_HOVER_ENTER,
		LEAVE = EVENT_GUI_HOVER_LEAVE
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
		GAINED = EVENT_GUI_FOCUS_GAINED,
		LOST = EVENT_GUI_FOCUS_LOST
	};

	FocusEvent(FocusEventType type) :
		Event(type)
	{
	}

private:
	
};

class UiObject;

class ResizeEvent : public Event
{
public:
	ResizeEvent(UiObject *uiObject);
	~ResizeEvent();

	uint getWidth() const
	{
		return m_width;
	}

	uint getHeight() const
	{
		return m_height;
	}

private:
	UiObject *const m_uiObject;
	const uint m_width, m_height;
};

#endif // UI_EVENTS_H