#include "Constants.h"
#include "Chat.h"
#include "Gui/GameOverlay.h"

Chat::Chat(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_active(false),
	m_chatInput(scene, this),
	m_fadeTime(0.0f),
	m_chatText(""),
	m_font(ResourceManager::get<Font>(UI_CHAT_FONT))
{
	setAnchor(Vector2(0.0f, 0.4f));
	setSize(Vector2(400.0f, 250.0f) / m_parent->getSize());
	setPosition(Vector2(0.01f, 0.0f));

	m_chatInput.setAnchor(Vector2(0.0f, 1.0f));
	m_chatInput.setSize(Vector2(400.0f, 27.0f) / getSize());
	m_chatInput.setPosition(Vector2(0.0f, 0.0f));
}

void Chat::toggle(int action)
{
	if(action != GLFW_PRESS) return;
	m_active = !m_active;

	if(m_active)
	{
		Input::setContext(Input::getContext("chat"));
		m_fadeTime = 0.0f;
	}
	else
	{
		Input::setContext(Input::getContext("game"));
		m_fadeTime = CHAT_FADE_TIME;
	}

	//Input::captureTextInput();
}

void Chat::sendMessage(int action)
{
	if(!m_active || action != GLFW_PRESS) return;

	string chatStr = m_chatInput.getText();
	if(!chatStr.empty())
	{
		if(chatStr[0] == '/')
		{
			//parseCommand(charStr.substr(1));
			m_chatText += chatStr.substr(1, chatStr.find_first_of(" ") - 2) + " is not a recognized command.";
		}
		else
		{
			// TODO: Get player name and insert
			m_chatText += "Bitsauce: " + chatStr;
		}
		m_chatText += "\n";
	}
	m_chatInput.setText("");
	toggle(action);
}

void Chat::update(float dt)
{
	m_chatInput.setActive(m_active);
	m_chatInput.update(dt);
	if(!m_active)
	{
		m_fadeTime -= dt;
	}
}

void Chat::draw(SpriteBatch *spriteBatch, float alpha)
{
	if(!m_active && m_fadeTime <= 0.0f) return;

	Vector2 position = getPosition();
	Vector2 size = getSize();

	float opacity = 1.0f;
	if(m_fadeTime > 0.0f)
	{
		opacity = min((m_fadeTime * 2.0f) / CHAT_FADE_TIME, 1.0f);
	}

	spriteBatch->getGraphicsContext().drawRectangle(position, size - Vector2(0.0f, 30.0f), Color(0, 0, 0, 127 * opacity));

	m_font->setColor(Color(255, 255, 255, 255 * opacity));
	m_font->draw(spriteBatch, position + Vector2(5.0f), m_chatText);

	if(m_fadeTime <= 0.0f)
	{
		m_chatInput.draw(spriteBatch);
	}
}