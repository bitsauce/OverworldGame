#include "Constants.h"
#include "Chat.h"
#include "GameOverlay.h"
#include "Game/Game.h"
#include "Entities/Player.h"

Chat::Chat(Overworld *game, GameOverlay *gameOverlay) :
	UiObject(gameOverlay),
	m_game(game),
	m_active(false),
	m_fadeTime(0.0f),
	m_font(Resource<Font>("Fonts/Chat")),
	m_redrawText(true),
	m_textSpriteBatch(),
	m_inputHistoryIndex(0)
{
	setAnchor(Vector2F(0.0f, 0.4f));
	setSize(Vector2F(CHAT_WIDTHF, CHAT_HEIGHTF) / gameOverlay->getDrawSize());
	setPosition(Vector2F(0.01f, 0.0f));

	m_chatInput = new LineEdit(this, CHAT_WIDTH, CHAT_TEXT_INPUT_HEIGHTF, m_font, Resource<Texture2D>("Gui/GameOverlay/Chat_Input"), Resource<Texture2D>("Gui/GameOverlay/Chat_Input"), 0);
	m_chatInput->setAnchor(Vector2F(0.0f, 1.0f + 3.0f / CHAT_HEIGHTF));
	m_chatInput->setSize(Vector2F(CHAT_WIDTHF, CHAT_TEXT_INPUT_HEIGHTF) / getDrawSize());
	m_chatInput->setPosition(Vector2F(0.0f, 0.0f));
	m_chatInput->setTextColor(Color(255, 255, 255, 255));

	m_chatRenderTarget = new RenderTarget2D(CHAT_WIDTH, CHAT_HEIGHT);
	m_font->setColor(Color(255, 255, 255, 255));
}

void Chat::toggle(InputEvent *e)
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

void Chat::sendMessage(InputEvent *e)
{
	if(!m_active || e->getType() != KeyEvent::DOWN) return;

	string chatStr = m_chatInput->getText();
	if(!chatStr.empty())
	{
		if(chatStr[0] == '/')
		{
			m_game->getCommander()->execute(chatStr.substr(1));
		}
		else
		{
			// Construct player chat message
			stringstream message;
			message << "rgb[255, 108, 108][";
			message << m_game->getClient()->getWorld()->getLocalPlayer()->getName();
			message << "]: " << chatStr;

			// Insert message and send to server
			insertMessage(message.str());
			m_game->getClient()->sendChatMessage(message.str());
		}
		m_inputHistory.push_back(chatStr);
		m_inputHistoryIndex = m_inputHistory.size();
	}
	m_chatInput->setText("");
	toggle(e);
}

void Chat::nextMessage(InputEvent *e)
{
	if(e->getType() != KeyEvent::DOWN) return;
	if(m_inputHistoryIndex + 1 < m_inputHistory.size())
	{
		m_chatInput->setText(m_inputHistory[++m_inputHistoryIndex]);
	}
}

void Chat::prevMessage(InputEvent *e)
{
	if(e->getType() != KeyEvent::DOWN) return;
	if(m_inputHistoryIndex - 1 >= 0)
	{
		m_chatInput->setText(m_inputHistory[--m_inputHistoryIndex]);
	}
}

void Chat::insertMessage(const string &message)
{
	m_messages.push_front(message);
	m_fadeTime = CHAT_FADE_TIME;
	m_redrawText = true;
}

void Chat::onTick(TickEvent *e)
{
	m_chatInput->setFocused(m_active);
	m_chatInput->onTick(e);
	if(!m_active)
	{
		m_fadeTime -= e->getDelta();
	}
}

struct ColoredMessage
{
	string plainText;
	list<tuple<uint, Color>> colors;
};

// rgb[255, 0, 0][Bitsauce:] rgb[0, 255, 0][Hello] rgb[0, 0, 255][World]!
// rgb[255, 108, 108][H] rgb[255, 177, 108][E] rgb[253, 255, 108][L] rgb[181, 255, 108][L] rgb[108, 255, 200][O] rgb[108, 255, 250][W] rgb[108, 225, 255][O] rgb[108, 180, 255][R] rgb[163, 108, 255][L] rgb[255, 108, 255][D] rgb[255, 108, 165][!]
// rgb[181, 255, 108][How] rgb[108, 225, 255][About] rgb[108, 180, 255][Some] rgb[255, 108, 255][Color]rgb[255, 108, 165][?]
// rgb[255, 108, 108][abcdefghijklmnopqrstuvwxyzabcdefghijklm]nopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
// rgb[255, 108, 108][abcdefghijklmnopqrstuvwxyz abcdefghijklm]nopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// Bitsauce: Hello World!
// colors[0] = (9, (255, 0, 0))
// colors[1] = (5, (0, 255, 0))
// colors[2] = (5, (0, 0, 255))
// colors[3] = (1, (255, 255, 255))

// TODO:
// - Create predefined colors: red, blue, green, yellow, lightblue, lightgreen, etc. (css colors?)
// - Add hex constructor to Color class
//   - Color(string) or Color::fromHex()
// - Syntax?
// rgb[r,g,b][msg]
// \red[msg]
// c{r,g,b}[msg]
// c{r,g,b,a}[msg]
// c{#hex}[msg]
// c[#hex]{msg}
// c[red]{msg}
// c[lightgreen]{msg}
// c[rainbow]{msg}
// - Fix text input text rendering. It's a bit off (something something premultiplied alpha)
// - White speck on username?
// - Crash on hard line break
// - Input while chat is active (debug arrows)
// - Add commander /help
// - Commander output should be color coded
ColoredMessage parseMessageColors(const string &message)
{
	ColoredMessage coloredMessage;
	int lastPos = 0;
	const Color defaultColor = Color::White;
	string plainText;
	for(int i = 0; i < message.size(); i++)
	{
		plainText += message[i];
		if(message[i] == '[' && message[i - 3] == 'r' && message[i - 2] == 'g' && message[i - 1] == 'b')
		{
			plainText = plainText.substr(0, plainText.size() - 4);
			if(!plainText.empty())
			{
				coloredMessage.plainText += plainText;
				coloredMessage.colors.push_back(make_tuple(plainText.size(), defaultColor));
				plainText.clear();
			}

			vector<int> rgbValues;
			string rgbString;
			for(i++; i < message.size(); i++)
			{
				if(message[i] == ',')
				{
					rgbValues.push_back(util::strToInt(rgbString));
					rgbString.clear();
				}
				else if(message[i] == ']')
				{
					rgbValues.push_back(util::strToInt(rgbString));
					break;
				}
				else if(message[i] != ' ')
				{
					rgbString += message[i];
				}
			}
			if(rgbValues.size() != 3) break;
			if(i >= message.size()) break;
			for(i++; i < message.size() && message[i] != '['; i++);
			if(i >= message.size()) break;
			for(i++; i < message.size() && message[i] != ']'; i++) { plainText += message[i]; }
			if(i >= message.size()) break;
			
			Color color(rgbValues[0], rgbValues[1], rgbValues[2]);
			coloredMessage.plainText += plainText;
			coloredMessage.colors.push_back(make_tuple(plainText.size(), color));
			plainText.clear();

			lastPos = i + 1;
		}
	}

	string lastText = message.substr(lastPos);
	coloredMessage.plainText += lastText;
	coloredMessage.colors.push_back(make_tuple(lastText.size(), defaultColor));

	return coloredMessage;
}

void Chat::onDraw(DrawEvent *e)
{
	if(!m_active && m_fadeTime <= 0.0f) return;

	Vector2I position = getDrawPosition();
	Vector2I size = getDrawSize();

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

		if(!m_messages.empty())
		{
			// Draw text to render target
			float lineHeight = m_font->getStringHeight(" ");
			int lineCount = (int) ceil(CHAT_HEIGHTF / lineHeight);
			context->enableScissor(0, 4.0f, size.x, size.y - 8);
			m_textSpriteBatch.begin(context, SpriteBatch::State(SpriteBatch::DEFERRED,
																BlendState(BlendState::BLEND_SRC_ALPHA,
																		   BlendState::BLEND_ONE_MINUS_SRC_ALPHA,
																		   BlendState::BLEND_ONE,
																		   BlendState::BLEND_ONE_MINUS_SRC_ALPHA)));

			// For each message in the chat
			list<string>::const_iterator itr = m_messages.begin();
			for(int i = 0; i < lineCount && itr != m_messages.end(); ++itr)
			{
				// Parse message colors
				ColoredMessage coloredMessage = parseMessageColors(*itr);
				
				// Set initial string colors
				list<tuple<uint, Color>>::const_iterator colorItr = coloredMessage.colors.begin();
				uint coloredTextLength = get<0>(*colorItr);
				Color textColor = get<1>(*colorItr);
				uint hardBreakCounter = 0;

				// Split plain text into lines that fit the chat box
				list<Font::BoxLine> lines = m_font->getBoxLines(size.x - 10.0f, coloredMessage.plainText, -1, FONT_ALIGN_LEFT);
				i += lines.size();

				// For each line the message was split into
				for(Font::BoxLine line : lines)
				{
					int linePos = 0;
					while(linePos < line.text.size())
					{
						// Draw the text with the appropriate color
						uint textCount = min(coloredTextLength, line.text.size() - linePos);
						m_font->setColor(textColor); // TODO: Check if a hard break happend, and consider the hyphen accordingly
						m_font->drawInternal(&m_textSpriteBatch,
											 5.0f + line.xoffset + m_font->getStringWidth(line.text.substr(0, linePos)),
											 size.y - i * lineHeight - 5.0f + line.yoffset,
											 &line.text[linePos],
											 textCount,
											 line.spacing);

						// We've drawn some of the line, add it to our text pos
						linePos += textCount;
						coloredTextLength -= textCount;

						// Get next piece of colored text
						if(coloredTextLength <= 0)
						{
							if(++colorItr == coloredMessage.colors.end()) break;
							coloredTextLength = get<0>(*colorItr);
							textColor = get<1>(*colorItr);
							if(line.breakType == Font::BoxLine::HARD &&
							   coloredTextLength >= line.text.size() - linePos)
							{
								coloredTextLength += 1;
							}
						}
					}

					if(line.breakType == Font::BoxLine::SOFT)
					{
						coloredTextLength--;
					}
				}
			}

			m_textSpriteBatch.end();
			context->disableScissor();
		}

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
	context->drawRectangle(position, size, Color(0, 0, 0, uchar(127 * opacity)));
	context->setTexture(m_chatRenderTarget->getTexture());
	context->setBlendState(BlendState(BlendState::PRESET_PREMULTIPLIED_ALPHA));
	context->drawRectangle(position, size, Color(255, 255, 255, uchar(255 * opacity)));
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
	context->setTexture(0);

	// If fade time == 0.0f we draw the chat text input
	if(m_fadeTime <= 0.0f)
	{
		m_chatInput->onDraw(e);
	}
}