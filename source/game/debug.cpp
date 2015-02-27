#include "Debug.h"
#include "Constants.h"
#include "Lighting/Lighting.h"
#include "Lighting/Spotlight.h"
#include "World/World.h"
#include "Entities/Camera.h"
#include "Terrain/Terrain.h"
#include "Blocks/BlockData.h"

Debug::Debug(Terrain *terrain, Lighting *lighting) :
	GameObject(DRAW_ORDER_DEBUG),
	m_terrain(terrain),
	m_lighting(lighting),
	m_block(BLOCK_GRASS),
	m_enabled(false),
	m_debugChunkLoader(false),
	m_font(ResourceManager::get<Font>(UI_DEBUG_FONT)),
	m_variables(),
	m_bulbSprite(ResourceManager::get<Texture2D>(":/sprites/debug/icon_bulb.png"))
{
	// Set font color
	m_font->setColor(Color(0, 0, 0, 255));

	m_bulbSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);

	// Debug binds
	Input::bind(XD_KEY_PERIOD, function<void()>(bind(&Debug::nextBlock, this)));
	Input::bind(XD_KEY_COMMA, function<void()>(bind(&Debug::prevBlock, this)));
	
	Input::bind(XD_KEY_F2, function<void()>(bind(&Debug::debugF2, this)));
	Input::bind(XD_KEY_F3, function<void()>(bind(&Debug::debugF3, this)));
	Input::bind(XD_KEY_F4, function<void()>(bind(&Debug::debugF4, this)));
	Input::bind(XD_KEY_F5, function<void()>(bind(&Debug::debugF5, this)));
}

void Debug::debugF2()
{
	toggle();
}

Random random;
void Debug::debugF3()
{
	new Spotlight(World::getCamera()->getInputPosition()/BLOCK_PXF, 20, Color((uchar)random.nextInt(255), (uchar)random.nextInt(255), (uchar)random.nextInt(255)));
}

void Debug::debugF4()
{
	m_debugChunkLoader = !m_debugChunkLoader;
	m_terrain->getChunkLoader()->m_applyZoom = !m_terrain->getChunkLoader()->m_applyZoom;
}

void Debug::debugF5()
{
	m_terrain->setThingAt(World::getCamera()->getInputPosition().x/BLOCK_PXF, World::getCamera()->getInputPosition().y/BLOCK_PXF, BLOCK_ENTITY_RED_CURRANT_BUSH);
}

//#include "BitStream.h"

void Debug::update()
{
	if(!m_enabled) return;

	// Block painting
	TerrainLayer layer = TERRAIN_LAYER_MIDDLE;
	if(Input::getKeyState(XD_KEY_LSHIFT)) layer = TERRAIN_LAYER_FRONT;
	if(Input::getKeyState(XD_KEY_LCONTROL)) layer = TERRAIN_LAYER_BACK;
	if(Input::getKeyState(XD_LMB))
	{
		int x = floor(World::getCamera()->getInputPosition().x/BLOCK_PXF), y = floor(World::getCamera()->getInputPosition().y/BLOCK_PXF);

		m_terrain->setBlockAt(x, y, m_block, layer);

		/*RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
		bitStream.Write(x);
		bitStream.Write(y);
		bitStream.Write(m_block);
		bitStream.Write(layer);*/
		//Server::getInstance()->sendPacket(bitStream);

	}
	else if(Input::getKeyState(XD_RMB))
	{
		m_terrain->setBlockAt(floor((World::getCamera()->getPosition().x + Input::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + Input::getPosition().y)/BLOCK_PXF), BLOCK_EMPTY, layer);
	}
}

void Debug::draw(SpriteBatch *spriteBatch)
{
	if(!m_enabled) return;
	
	setVariable("Chunks", util::intToStr(m_terrain->getChunkLoader()->m_chunks.size()) + " / " + util::intToStr(m_terrain->getChunkLoader()->m_optimalChunkCount));
	//setVariable("Time", util::intToStr(m_timeOfDay->getHour()) + ":" + util::intToStr(m_timeOfDay->getMinute()));

	// Draw debug info
	string drawString;
	for(map<string, string>::iterator itr = m_variables.begin(); itr != m_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}
	m_font->draw(spriteBatch, Vector2(0.0f), drawString);

	// Block painter
	spriteBatch->drawText(Vector2(5.0f, Window::getSize().y - 48.0f), "Current block:    (" + util::intToStr(m_block) + ")\n" + "Current layer: " + (Input::getKeyState(XD_KEY_LCONTROL) ? "BACK" : (Input::getKeyState(XD_KEY_LSHIFT) ? "FRONT" : "SCENE")), m_font);
	Sprite blockSprite(BlockData::get(m_block).getTexture(), Rect(150.0f, Window::getSize().y - 58.0f, 32.0f, 32.0f), Vector2(0.0f, 0.0f), 0.0f, TextureRegion(0.0f, 0.0f, 1.0f, 0.6f));
	spriteBatch->drawSprite(blockSprite);
	
	// Draw block grid
	GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
	gfxContext.setTexture(nullptr);
	if(Input::getKeyState(XD_KEY_B))
	{
		int x0 = (int)floor(World::getCamera()->getX()/BLOCK_PXF);
		int y0 = (int)floor(World::getCamera()->getY()/BLOCK_PXF);
		int x1 = (int)floor((World::getCamera()->getX() + World::getCamera()->getWidth())/BLOCK_PXF);
		int y1 = (int)floor((World::getCamera()->getY() + World::getCamera()->getHeight())/BLOCK_PXF);

		for(int y = y0; y <= y1; ++y)
		{
			gfxContext.drawRectangle(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / World::getCamera()->getZoomLevel(), Color(127, 127, 127, 255));
		}

		for(int x = x0; x <= x1; ++x)
		{
			gfxContext.drawRectangle(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / World::getCamera()->getZoomLevel(), (y1 - y0 + 1) * BLOCK_PXF, Color(127, 127, 127, 255));
		}
	}

	// Draw chunk grid
	Vector2 position = World::getCamera()->getPosition();
	Vector2 size = World::getCamera()->getSize();
	int x0 = (int)floor(position.x/CHUNK_PXF);
	int y0 = (int)floor(position.y/CHUNK_PXF);
	int x1 = (int)floor((position.x+size.x)/CHUNK_PXF);
	int y1 = (int)floor((position.y+size.y)/CHUNK_PXF);

	for(int y = y0; y <= y1; ++y)
	{
		gfxContext.drawRectangle(x0 * CHUNK_PXF, y * CHUNK_PXF, (x1 - x0 + 1) * CHUNK_PXF, 1.0f / World::getCamera()->getZoomLevel(), Color(0, 0, 0, 255));
	}

	for(int x = x0; x <= x1; ++x)
	{
		gfxContext.drawRectangle(x * CHUNK_PXF, y0 * CHUNK_PXF, 1.0f / World::getCamera()->getZoomLevel(), (y1 - y0 + 1) * CHUNK_PXF, Color(0, 0, 0, 255));
	}

	if(m_debugChunkLoader)
	{
		// Draw current view
		Vector2 position = World::getCamera()->getCenter() - Window::getSize()*0.5f;
		Vector2 size = Window::getSize();
		gfxContext.drawRectangle(position.x, position.y, 1.0f / World::getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		gfxContext.drawRectangle((position.x+size.x), position.y, 1.0f / World::getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		gfxContext.drawRectangle(position.x, position.y, size.x, 1.0f / World::getCamera()->getZoomLevel(), Color(127, 127, 255, 255));
		gfxContext.drawRectangle(position.x, (position.y+size.y), size.x, 1.0f / World::getCamera()->getZoomLevel(), Color(127, 127, 255, 255));

		// Draw active area
		ChunkLoader::ChunkArea activeArea = m_terrain->getChunkLoader()->getActiveArea();
		gfxContext.drawRectangle(activeArea.x0 * CHUNK_PX, activeArea.y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), (activeArea.y1 - activeArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle((activeArea.x1+1) * CHUNK_PX, activeArea.y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), (activeArea.y1 - activeArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle(activeArea.x0 * CHUNK_PX, activeArea.y0 * CHUNK_PX, (activeArea.x1 - activeArea.x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), Color(255, 255, 255, 255));
		gfxContext.drawRectangle(activeArea.x0 * CHUNK_PX, (activeArea.y1+1) * CHUNK_PX, (activeArea.x1 - activeArea.x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), Color(255, 255, 255, 255));

		// Draw load area
		ChunkLoader::ChunkArea loadArea = m_terrain->getChunkLoader()->getLoadArea();
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, loadArea.y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle((loadArea.x1+1) * CHUNK_PX, loadArea.y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, loadArea.y0 * CHUNK_PX, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), Color(255, 255, 255, 255));
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, (loadArea.y1+1) * CHUNK_PX, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoomLevel(), Color(255, 255, 255, 255));

		for(int y = y0; y <= y1; ++y)
		{
			for(int x = x0; x <= x1; ++x)
			{
				if(!m_terrain->getChunkLoader()->isChunkLoadedAt(x, y))
				{
					gfxContext.drawRectangle(x * CHUNK_PX, y * CHUNK_PX, CHUNK_PX, CHUNK_PX, Color(255, 0, 0, 127));
				}
				else if(m_terrain->getChunkLoader()->getChunkAt(x, y).isDirty(TERRAIN_LAYER_MIDDLE))
				{
					gfxContext.drawRectangle(x * CHUNK_PX, y * CHUNK_PX, CHUNK_PX, CHUNK_PX, Color(0, 0, 255, 127));
				}
			}
		}
	}

	spriteBatch->end();
	
	// Show lighting passes
	if(Input::getKeyState(XD_KEY_L))
	{
		gfxContext.disable(GraphicsContext::BLEND);
		spriteBatch->begin();
		spriteBatch->drawSprite(Sprite(m_lighting->m_lightingRenderTarget->getTexture(), Rect(0.0f, 128.0f, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_lighting->m_lightingPass0->getTexture(), Rect(0.0f, 128.0f*2, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_lighting->m_lightingPass1->getTexture(), Rect(0.0f, 128.0f*3, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_lighting->m_lightingPass2->getTexture(), Rect(0.0f, 128.0f*4, 256.0f, 128.0f)));
		spriteBatch->end();
		gfxContext.enable(GraphicsContext::BLEND);
	}

	// Show light sources as light bulbs
	spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, World::getCamera()->getProjectionMatrix()));
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
		m_block = BlockID(BLOCK_ENTITY + 1);
	}
}

void Debug::prevBlock()
{
	if(!m_enabled) return;
	if((m_block = BlockID(m_block - 1)) <= BLOCK_ENTITY)
	{
		m_block = BlockID(BLOCK_COUNT - 1);
	}
}

void Debug::setVariable(const string &name, const string &value)
{
	m_variables[name] = value;
}