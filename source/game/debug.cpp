#include "debug.h"
#include "game.h"

Debug::Debug(Terrain *terrain, Lighting *lighting) :
	GameObject(DRAW_ORDER_DEBUG),
	m_terrain(terrain),
	m_lighting(lighting),
	m_block(BLOCK_GRASS),
	m_enabled(false),
	m_font(xd::ResourceManager::get<xd::Font>(UI_DEBUG_FONT)),
	m_variables(),
	m_bulbSprite(xd::ResourceManager::get<xd::Texture2D>(":/sprites/debug/icon_bulb.png"))
{
	// Set font color
	m_font->setColor(xd::Color(0, 0, 0, 255));

	m_bulbSprite.setRegion(xd::TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);

	// Debug binds
	xd::Input::bind(xd::XD_KEY_F2, function<void()>(bind(&Debug::toggle, this)));
	xd::Input::bind(xd::XD_KEY_PERIOD, function<void()>(bind(&Debug::nextBlock, this)));
	xd::Input::bind(xd::XD_KEY_COMMA, function<void()>(bind(&Debug::prevBlock, this)));
	
	xd::Input::bind(xd::XD_KEY_F3, function<void()>(bind(&Debug::debugF3, this)));
	xd::Input::bind(xd::XD_KEY_F4, function<void()>(bind(&Debug::debugF4, this)));
}

xd::Random random;
void Debug::debugF3()
{
	new Spotlight((World::getCamera()->getPosition() + xd::Input::getPosition())/BLOCK_PXF, 20, xd::Color(random.nextInt(255), random.nextInt(255), random.nextInt(255)));
}

void Debug::debugF4()
{

}

void Debug::update()
{
	if(!m_enabled) return;

	// Block painting
	TerrainLayer layer = TERRAIN_LAYER_MIDDLE;
	if(xd::Input::getKeyState(xd::XD_KEY_LSHIFT)) layer = TERRAIN_LAYER_FRONT;
	if(xd::Input::getKeyState(xd::XD_KEY_LCONTROL)) layer = TERRAIN_LAYER_BACK;
	if(xd::Input::getKeyState(xd::XD_LMB))
	{
		m_terrain->setBlockAt(floor((World::getCamera()->getPosition().x + xd::Input::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + xd::Input::getPosition().y)/BLOCK_PXF), m_block, layer);
	}
	else if(xd::Input::getKeyState(xd::XD_RMB))
	{
		m_terrain->setBlockAt(floor((World::getCamera()->getPosition().x + xd::Input::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + xd::Input::getPosition().y)/BLOCK_PXF), BLOCK_EMPTY, layer);
	}
}

void Debug::draw(xd::SpriteBatch *spriteBatch)
{
	if(!m_enabled) return;

	// Draw debug info
	string drawString;
	for(map<string, string>::iterator itr = m_variables.begin(); itr != m_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}
	m_font->draw(spriteBatch, Vector2(5.0f, 48.0f), drawString);

	// Block painter
	spriteBatch->drawText(Vector2(5.0f, xd::Window::getSize().y - 48.0f), "Current block:    (" + xd::util::intToStr(m_block) + ")\n" + "Current layer: " + (xd::Input::getKeyState(xd::XD_KEY_LCONTROL) ? "BACK" : (xd::Input::getKeyState(xd::XD_KEY_LSHIFT) ? "FRONT" : "SCENE")), m_font);
	xd::Sprite blockSprite(BlockData::get(m_block).getTexture(), Rect(150.0f, xd::Window::getSize().y - 58.0f, 32.0f, 32.0f), Vector2(0.0f, 0.0f), 0.0f, xd::TextureRegion(0.0f, 0.0f, 1.0f, 0.6f));
	spriteBatch->drawSprite(blockSprite);
	
	int x0 = floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
	
	// Draw block grid
	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
	gfxContext.setTexture(nullptr);
	if(xd::Input::getKeyState(xd::XD_KEY_B))
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

	// Draw chunk grid
	for(int y = y0; y <= y1; ++y)
	{
		gfxContext.drawRectangle(x0 * CHUNK_PX, y * CHUNK_PX, (x1 - x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), xd::Color(0, 0, 0, 255));
	}

	for(int x = x0; x <= x1; ++x)
	{
		gfxContext.drawRectangle(x * CHUNK_PX, y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * CHUNK_PX, xd::Color(0, 0, 0, 255));
	}
	spriteBatch->end();
	
	// Show lighting passes
	if(xd::Input::getKeyState(xd::XD_KEY_L))
	{
		gfxContext.disable(xd::GraphicsContext::BLEND);
		spriteBatch->begin();
		spriteBatch->drawSprite(xd::Sprite(m_lighting->m_lightingRenderTarget->getTexture(), Rect(0.0f, 128.0f, 256.0f, 128.0f)));
		spriteBatch->drawSprite(xd::Sprite(m_lighting->m_lightingPass0->getTexture(), Rect(0.0f, 128.0f*2, 256.0f, 128.0f)));
		spriteBatch->drawSprite(xd::Sprite(m_lighting->m_lightingPass1->getTexture(), Rect(0.0f, 128.0f*3, 256.0f, 128.0f)));
		spriteBatch->drawSprite(xd::Sprite(m_lighting->m_lightingPass2->getTexture(), Rect(0.0f, 128.0f*4, 256.0f, 128.0f)));
		spriteBatch->end();
		gfxContext.enable(xd::GraphicsContext::BLEND);
	}

	// Show loaded chunks (red: unloaded, green: loaded, blue: loaded and current)
	if(xd::Input::getKeyState(xd::XD_KEY_C))
	{
		int w = floor(xd::Window::getSize().x/10.0f)/2.0f;
		int h = floor(xd::Window::getSize().y/10.0f)/2.0f;
		for(int y = -h; y <= h; ++y)
		{
			for(int x = -w; x <= w; ++x)
			{
				xd::Color color(255, 0, 0);
				if(x == 0 && y == 0)
				{
					color = xd::Color(0, 0, 255);
				}
				else if(m_terrain->isChunk(x0 + x, y0 + y))
				{
					color = xd::Color(0, 255, 0);
				}
				color.a = 127;
				gfxContext.drawRectangle((x+w)*10, (y+h)*10, 10, 10, color);
			}
		}
	}

	// Show light sources as light bulbs
	spriteBatch->begin(xd::SpriteBatch::State(xd::SpriteBatch::DEFERRED, xd::BlendState::PRESET_ALPHA_BLEND, World::getCamera()->getProjectionMatrix()));
	for(list<LightSource*>::iterator itr = m_lighting->m_lightSources.begin(); itr != m_lighting->m_lightSources.end(); ++itr)
	{
		m_bulbSprite.setPosition((*itr)->getPosition()*BLOCK_PXF + Vector2(0.5f, 0.5f));
		spriteBatch->drawSprite(m_bulbSprite);
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