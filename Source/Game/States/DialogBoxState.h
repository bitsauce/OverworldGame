#pragma once

#include "Config.h"
#include "GameState.h"
#include "Gui/UiObjects/Button.h"
#include "Gui/UiObjects/Label.h"
#include "Gui/UiObjects/Canvas.h"
#include "Gui/UiObjects/AspectRatioContainer.h"

class Overworld;

class DialogBoxState : public GameState
{
public:
	DialogBoxState(Overworld *gui, Window *window, const string &message/*, Type type = DIALOG_BOX_OK*/);

private:
	AspectRatioContainer *m_aspectRatioContainer;
	Button *m_buttonOk;
	Label *m_message;
};