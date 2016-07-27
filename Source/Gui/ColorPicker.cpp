#include "ColorPicker.h"

ColorPicker::ColorPicker(UiObject *parent) :
	UiObject(parent),
	m_colorWheelShader(Game::GetInstance()->getResourceManager()->get<Shader>("Shaders/Color_Wheel")),
	m_ringSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Gui/Color_Picker_Ring")),
	m_ringLocation(Vector2F(0.0f)),
	m_renderTarget(0),
	m_leftMouseButton(false)
{
	m_ringSprite.setRegion(TextureRegion(), true);
	m_ringSprite.setOrigin(Vector2F(m_ringSprite.getSize() * 0.5f));

	m_colorWheelShader->setUniform1f("u_Value", 1.0f);
}

Color ColorPicker::getSelectedColor() const
{
	return m_selectedColor;
}

void ColorPicker::onTick(TickEvent *e)
{
}

void ColorPicker::onDraw(DrawEvent *e)
{
	RectI rect = getDrawRect();
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();

	GraphicsContext *context = e->getGraphicsContext();
	context->setRenderTarget(m_renderTarget);
	context->setShader(m_colorWheelShader);
	context->drawRectangle(0.0f, 0.0f, rect.getWidth(), rect.getHeight());
	context->setShader(0);
	context->setRenderTarget(0);

	context->setTexture(m_renderTarget->getTexture());
	context->drawRectangle(rect);
	context->setTexture(0);


	m_ringSprite.setPosition(rect.position + rect.size * 0.5f * (m_ringLocation + Vector2F(1.0f)));
	spriteBatch->drawSprite(m_ringSprite);
}

void ColorPicker::onResize(ResizeEvent *e)
{
	delete m_renderTarget;
	m_renderTarget = new RenderTarget2D(e->getWidth(), e->getHeight());
}

void ColorPicker::onMouseEvent(MouseEvent *e)
{
	switch(e->getType())
	{
		case MouseEvent::DOWN:
		{
			if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT)
			{
				m_leftMouseButton = true;
			}
		}
		break;

		case MouseEvent::UP:
		{
			if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT)
			{
				m_leftMouseButton = false;
			}
			return;
		}
		break;
	}

	RectI rect = getDrawRect();
	if(m_leftMouseButton && rect.contains(e->getPosition()))
	{
		m_ringLocation = Vector2F(e->getPosition() - rect.position - rect.size * 0.5f) / Vector2F(rect.size * 0.5f);
		if(m_ringLocation.length() > 1.0f)
		{
			m_ringLocation.normalize();
		}

		RectI rect = getDrawRect();
		Vector2I pos = rect.size * 0.5f * (m_ringLocation + Vector2F(1.0f));
		m_renderTarget->getTexture()->getPixmap().getPixel(pos.x, pos.y, &m_selectedColor);
	}
}
