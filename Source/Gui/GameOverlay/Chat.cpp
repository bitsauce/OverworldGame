#include "Constants.h"
#include "Chat.h"
#include "GameOverlay.h"
#include "Game/Game.h"
#include "Entities/Player.h"

Chat::Chat(OverworldGame *game, GraphicsContext *context, GameOverlay *gameOverlay) :
	UiObject(gameOverlay),
	m_game(game),
	m_active(false),
	m_chatInput(context, this),
	m_fadeTime(0.0f),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>("Fonts/Chat")),
	m_redrawText(true),
	m_textSpriteBatch(context),
	m_messages(100),
	m_chatLinePos(0),
	m_inputHistoryIndex(0)
{
	setAnchor(Vector2F(0.0f, 0.4f));
	setSize(Vector2F(CHAT_WIDTHF, CHAT_HEIGHTF) / gameOverlay->getDrawSize());
	setPosition(Vector2F(0.01f, 0.0f));

	m_chatInput.setAnchor(Vector2F(0.0f, 1.0f));
	m_chatInput.setSize(Vector2F(CHAT_WIDTHF, CHAT_TEXT_INPUT_HEIGHTF) / getDrawSize());
	m_chatInput.setPosition(Vector2F(0.0f, 0.0f));
	
	m_chatRenderTarget = new RenderTarget2D(CHAT_WIDTH, CHAT_HEIGHT);
	m_font->setColor(Color(255, 255, 255, 255));
}

void Chat::toggle(KeyEvent *e)
{
	if(e->getType() != KeyEvent::DOWN) return;

	m_active = !m_active;

	if(m_active)
	{
		m_game->getInputManager()->setContext(m_game->getInputManager()->getContextByName("chat"));
		m_fadeTime = 0.0f;
	}
	else
	{
		m_game->getInputManager()->setContext(m_game->getInputManager()->getContextByName("game"));
		m_fadeTime = CHAT_FADE_TIME;
	}
}

void Chat::sendMessage(KeyEvent *e)
{
	if(!m_active || e->getType() != KeyEvent::DOWN) return;

	string chatStr = m_chatInput.getText();
	if(!chatStr.empty())
	{
		if(chatStr[0] == '/')
		{
			m_game->getCommander()->execute(chatStr.substr(1));
		}
		else
		{
			insertMessage(m_game->getWorld()->getLocalPlayer()->getName() + ": " + chatStr);
		}
		m_redrawText = true;
		m_inputHistory.push_back(chatStr);
		m_inputHistoryIndex = m_inputHistory.size();
	}
	m_chatInput.setText("");
	toggle(e);
}

void Chat::nextMessage(KeyEvent *e)
{
	if(e->getType() != KeyEvent::DOWN) return;
	if(m_inputHistoryIndex + 1 < m_inputHistory.size())
	{
		m_chatInput.setText(m_inputHistory[++m_inputHistoryIndex]);
	}
}

void Chat::prevMessage(KeyEvent *e)
{
	if(e->getType() != KeyEvent::DOWN) return;
	if(m_inputHistoryIndex - 1 >= 0)
	{
		m_chatInput.setText(m_inputHistory[--m_inputHistoryIndex]);
	}
}

void Chat::insertMessage(const string &message)
{
	m_messages[m_chatLinePos++ % 100] = message;
}

void Chat::onTick(TickEvent *e)
{
	//setSize(Vector2F(CHAT_WIDTHF, CHAT_HEIGHTF) / ((UiObject*)getParent())->getDrawSize());
	//m_chatInput.setSize(Vector2F(CHAT_WIDTHF, CHAT_TEXT_INPUT_HEIGHTF) / getDrawSize());

	m_chatInput.setFocused(m_active);
	m_chatInput.onTick(e);
	if(!m_active)
	{
		m_fadeTime -= e->getDelta();
	}

	//UiObject::onTick(e);
}

void Chat::onDraw(DrawEvent *e)
{
	if(!m_active && m_fadeTime <= 0.0f) return;

	Vector2I position = getDrawPosition();
	Vector2I size = Vector2F(CHAT_WIDTHF, CHAT_HEIGHTF);

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	GraphicsContext *context = e->getGraphicsContext();

	if(m_redrawText)
	{
		// Set render target
		context->setRenderTarget(m_chatRenderTarget);

		// Clear render target
		context->setBlendState(BlendState(BlendState::BLEND_ZERO, BlendState::BLEND_ZERO));
		context->drawRectangle(Vector2F(0.0f, 0.0f), size);
		context->setBlendState(BlendState::PRESET_ALPHA_BLEND);

		// Create chat string
		string chatStr;
		int lineCount = (int) ceil(CHAT_HEIGHTF / m_font->getStringHeight(" "));
		for(int i = 0; i < lineCount; ++i)
		{
			chatStr += m_messages[i] + "\n";
		}

		// Draw text to render target
		m_textSpriteBatch.begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ONE)));
		m_font->drawBox(&m_textSpriteBatch, Vector2F(5.0f), size.x - 10.0f, chatStr, 0, FONT_ALIGN_JUSTIFY);
		m_textSpriteBatch.end();

		// Reset render target
		context->setRenderTarget(0);

		// Set redraw flag
		m_redrawText = false;
	}

	// Get fade opacity
	float opacity = 1.0f;
	if(m_fadeTime > 0.0f)
	{
		opacity = min((m_fadeTime * 2.0f) / CHAT_FADE_TIME, 1.0f);
	}

	// Draw chat rectangle and text
	context->drawRectangle(position, size - Vector2F(0.0f, 30.0f), Color(0, 0, 0, uchar(127 * opacity)));
	context->setTexture(m_chatRenderTarget->getTexture());
	context->drawRectangle(position, size, Color(255, 255, 255, uchar(255 * opacity)));
	context->setTexture(0);

	// If fade time == 0.0f we draw the chat text input
	if(m_fadeTime <= 0.0f)
	{
		m_chatInput.onDraw(e);
	}
}