#include "debug.h"
#include "game.h"

Debug::Debug(Terrain *terrain) :
	GameObject(DRAW_ORDER_DEBUG),
	m_terrain(terrain),
	m_block(BLOCK_GRASS),
	m_enabled(false),
	m_font(xd::ResourceManager::get<xd::Font>(UI_DEBUG_FONT)),
	m_variables()
{
	// Set font color
	m_font->setColor(xd::Color(0, 0, 0, 255));

	// Debug binds
	XInput::bind(XD_KEY_F2, function<void()>(bind(&Debug::toggle, this)));
	XInput::bind(XD_KEY_PERIOD, function<void()>(bind(&Debug::nextBlock, this)));
	XInput::bind(XD_KEY_COMMA, function<void()>(bind(&Debug::prevBlock, this)));
}

void Debug::update()
{
	if(!m_enabled) return;

	// Block painting
	TerrainLayer layer = TERRAIN_LAYER_MIDDLE;
	if(XInput::getKeyState(XD_KEY_LSHIFT)) layer = TERRAIN_LAYER_FRONT;
	if(XInput::getKeyState(XD_KEY_LCONTROL)) layer = TERRAIN_LAYER_BACK;
	if(XInput::getKeyState(XD_LMB))
	{
		m_terrain->setBlockAt(floor((World::getCamera()->getPosition().x + XInput::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + XInput::getPosition().y)/BLOCK_PXF), m_block, layer);
	}
	else if(XInput::getKeyState(XD_RMB))
	{
		m_terrain->setBlockAt(floor((World::getCamera()->getPosition().x + XInput::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + XInput::getPosition().y)/BLOCK_PXF), BLOCK_EMPTY, layer);
	}
}

void Debug::draw(xd::SpriteBatch *spriteBatch)
{
	if(!m_enabled) return;

	string drawString;
	for(map<string, string>::iterator itr = m_variables.begin(); itr != m_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}

	m_font->draw(spriteBatch, Vector2(5.0f, 48.0f), drawString);

	spriteBatch->drawText(Vector2(5.0f, XWindow::getSize().y - 48.0f), "Current block:    (" + util::intToStr(m_block) + ")\n" + "Current layer: " + (XInput::getKeyState(XD_KEY_LCONTROL) ? "BACK" : (XInput::getKeyState(XD_KEY_LSHIFT) ? "FRONT" : "SCENE")), m_font);
	xd::Sprite blockSprite(BlockData::get(m_block).getTexture(), Rect(150.0f, XWindow::getSize().y - 58.0f, 32.0f, 32.0f), Vector2(0.0f, 0.0f), 0.0f, xd::TextureRegion(0.0f, 0.0f, 1.0f, 0.6f));
	spriteBatch->drawSprite(blockSprite);
	
	int x0 = floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
	
	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
	gfxContext.setTexture(nullptr);
	if(XInput::getKeyState(XD_KEY_B))
	{
		int x0 = floor(World::getCamera()->getX()/BLOCK_PXF);
		int y0 = floor(World::getCamera()->getY()/BLOCK_PXF);
		int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/BLOCK_PXF);
		int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/BLOCK_PXF);

		for(int y = y0; y <= y1; ++y)
		{
			gfxContext.drawRectangle(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / World::getCamera()->getZoom(), xd::Color(127, 127, 127, 255));
		}

		for(int x = x0; x <= x1; ++x)
		{
			gfxContext.drawRectangle(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * BLOCK_PXF, xd::Color(127, 127, 127, 255));
		} 
	}

	for(int y = y0; y <= y1; ++y)
	{
		gfxContext.drawRectangle(x0 * CHUNK_PX, y * CHUNK_PX, (x1 - x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), xd::Color(0, 0, 0, 255));
	}

	for(int x = x0; x <= x1; ++x)
	{
		gfxContext.drawRectangle(x * CHUNK_PX, y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * CHUNK_PX, xd::Color(0, 0, 0, 255));
	}
}

void Debug::toggle()
{
	m_enabled = !m_enabled;
}

void Debug::nextBlock()
{
	if(!m_enabled) return;
	if((m_block = BlockID(m_block + 1)) >= BLOCK_COUNT)
	{
		m_block = BlockID(BLOCK_OCCUPIED + 1);
	}
}

void Debug::prevBlock()
{
	if(!m_enabled) return;
	if((m_block = BlockID(m_block - 1)) <= BLOCK_OCCUPIED)
	{
		m_block = BlockID(BLOCK_COUNT - 1);
	}
}

void Debug::setVariable(const string &name, const string &value)
{
	m_variables[name] = value;
}