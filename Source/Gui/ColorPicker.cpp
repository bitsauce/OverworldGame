#include "ColorPicker.h"

ColorPicker::ColorPicker(UiObject *parent) :
	UiObject(parent),
	m_ringSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Gui/Color_Picker_Ring")),
	m_colorWheelTexture(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Gui/Color_Wheel")),
	m_ringLocation(Vector2F(0.0f)),
	m_renderTarget(0),
	m_leftMouseButton(false),
	m_renderColorWheel(true),
	m_selectedAlpha(255),
	m_selectedColor(255, 255, 255)
{
	m_ringSprite.setRegion(TextureRegion(), true);
	m_ringSprite.setOrigin(Vector2F(m_ringSprite.getSize() * 0.5f));
	m_ringSprite.setDepth(1.0f);
}

float colorDiff(const ColorRGB &c1, const ColorRGB &c2)
{
	return sqrt(pow((c2.getR() - c1.getR()), 2.0) + pow((c2.getG() - c1.getG()), 2) + pow((c2.getB() - c1.getB()), 2));
}

void ColorPicker::setSelectedColor(const Color &color)
{
	map<ColorRGB, Vector2I>::iterator selectedColor;
	map<ColorRGB, Vector2I>::iterator upper = m_colorToCoordMap.lower_bound(color.getRGB());
	if(upper == m_colorToCoordMap.begin() || (upper != m_colorToCoordMap.end() && upper->first == color.getRGB()))
	{
		selectedColor = upper;
	}
	else
	{
		map<ColorRGB, Vector2I>::iterator lower = upper;
		--lower;
		if(upper == m_colorToCoordMap.end() || colorDiff(color.getRGB(), lower->first) < colorDiff(upper->first, color.getRGB()))
		{
			selectedColor = lower;
		}
		else
		{
			selectedColor = upper;
		}
	}

	m_selectedColor = selectedColor->first;
	m_selectedAlpha = color.getA();
	m_ringLocation = (Vector2F(selectedColor->second) / m_renderTarget->getTexture()->getSize()) * 2.0f - Vector2F(1.0f);
}

Color ColorPicker::getSelectedColor() const
{
	return Color(m_selectedColor, m_selectedAlpha);
}

void ColorPicker::onTick(TickEvent *e)
{
}

void ColorPicker::onDraw(DrawEvent *e)
{
	RectI rect = getDrawRect();
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	
	GraphicsContext *context = e->getGraphicsContext();
	if(m_renderColorWheel)
	{
		context->setRenderTarget(m_renderTarget);
		context->setTexture(m_colorWheelTexture);
		context->drawRectangle(0, 0, m_renderTarget->getWidth(), m_renderTarget->getHeight());
		context->setRenderTarget(0);

		Pixmap pixmap = m_renderTarget->getTexture()->getPixmap();
		for(uint y = 0; y < pixmap.getHeight(); y++)
		{
			for(uint x = 0; x < pixmap.getWidth(); x++)
			{
				uchar pixel[4];
				pixmap.getPixel(x, y, pixel);
				if(pixel[3] > 0)
				{
					ColorRGB color(pixel[0], pixel[1], pixel[2]);
					Vector2I coord(x, y);

					m_colorToCoordMap[color] = coord;
					m_coordToColorMap[coord] = color;
				}
			}
		}

		m_renderColorWheel = false;
	}

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
	m_renderColorWheel = true;
}

void ColorPicker::onMouseEvent(MouseEvent *e)
{
	RectI rect = getDrawRect();
	switch(e->getType())
	{
		case MouseEvent::DOWN:
		{
			if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT && rect.contains(e->getPosition()))
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

	if(m_leftMouseButton)
	{
		m_ringLocation = Vector2F(e->getPosition() - rect.position - rect.size * 0.5f) / Vector2F(rect.size * 0.5f);
		if(m_ringLocation.length() > 0.98f)
		{
			m_ringLocation = m_ringLocation.normalized() * 0.98f;
		}

		RectI rect = getDrawRect();
		Vector2I pos = rect.size * 0.5f * (m_ringLocation + Vector2F(1.0f));
		m_selectedColor = m_coordToColorMap[pos];
	}
}
