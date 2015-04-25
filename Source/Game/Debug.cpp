#include "BitStream.h"

#include "Debug.h"
#include "Constants.h"
#include "World/World.h"
#include "World/Camera.h"
#include "Entities/Player.h"
#include "Blocks/BlockData.h"
#include "Networking/Server.h"
#include "Entities/Mobs/Zombie.h"
#include "Scenes/Multiplayer.h"
#include "Scenes/GameScene.h"
#include "Game/GameStates/GameState.h"
#include "Game/Game.h"

Debug::Debug(Game *game) :
	m_game(game),
	m_world(game->getWorld()),
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
	
	Input::bind(XD_KEY_F1, function<void()>(bind(&Debug::debugFunction, this, 1)));
	Input::bind(XD_KEY_F2, function<void()>(bind(&Debug::debugFunction, this, 2)));
	Input::bind(XD_KEY_F3, function<void()>(bind(&Debug::debugFunction, this, 3)));
	Input::bind(XD_KEY_F4, function<void()>(bind(&Debug::debugFunction, this, 4)));
	Input::bind(XD_KEY_F5, function<void()>(bind(&Debug::debugFunction, this, 5)));
	Input::bind(XD_KEY_F6, function<void()>(bind(&Debug::debugFunction, this, 6)));
	Input::bind(XD_KEY_F7, function<void()>(bind(&Debug::debugFunction, this, 7)));
	Input::bind(XD_KEY_F8, function<void()>(bind(&Debug::debugFunction, this, 8)));
	Input::bind(XD_KEY_F9, function<void()>(bind(&Debug::debugFunction, this, 9)));
	Input::bind(XD_KEY_F10, function<void()>(bind(&Debug::debugFunction, this, 10)));
	Input::bind(XD_KEY_F11, function<void()>(bind(&Debug::debugFunction, this, 11)));
	Input::bind(XD_KEY_F12, function<void()>(bind(&Debug::debugFunction, this, 12)));
}

void Debug::debugFunction(const int i)
{
	switch(i)
	{
	case 1:
		{
			// Toggle debug info
			toggle();
		}
		break;
		
	case 2:
		{
			// Attach/detach camera
			Camera *camera = m_world->getCamera();
			if(camera->getTargetEntity())
			{
				camera->setTargetEntity(nullptr);
			}
			else
			{
				camera->setTargetEntity(m_world->getLocalPlayer());
			}
		}
		break;
		
	case 3:
		{
			// Toggle lighting
			m_world->getLighting()->m_enabled = !m_world->getLighting()->m_enabled;
		}
		break;
		
	case 4:
		{
			// Toggle chunk loader debugging
			m_debugChunkLoader = !m_debugChunkLoader;
			m_world->getTerrain()->getChunkLoader()->m_applyZoom = !m_world->getTerrain()->getChunkLoader()->m_applyZoom;
		}
		break;
		
	case 5:
		{
			// Spawn light
			new Spotlight(m_world->getLighting(), m_world->getCamera()->getInputPosition()/BLOCK_PXF, 20, Color((uchar)m_random.nextInt(255), (uchar)m_random.nextInt(255), (uchar)m_random.nextInt(255)));
			// Spawn zombie
			//Zombie *zombie = new Zombie();
			//zombie->getBody().setPosition(0, 0);
		}
		break;
		
	case 6:
		{
		}
		break;
		
	case 7:
		{
		}
		break;
		
	case 8:
		{
		}
		break;
		
	case 9:
		{
		}
		break;
		
	case 10:
		{
		}
		break;
		
	case 11:
		{
			if(m_game->peekState()->getID() == GAME_STATE_MULTIPLAYER)
			{
				m_game->popState();
			}
			else
			{
				//m_game.pushState(new MultiplayerScene(m_world));
			}
		}
		break;
		
	case 12:
		{
		}
		break;
	}
}

void Debug::update()
{
	if(!m_enabled) return;
	/*
	// Block painting
	TerrainLayer layer = TERRAIN_LAYER_MIDDLE;
	if(Input::getKeyState(XD_KEY_LSHIFT)) layer = TERRAIN_LAYER_FRONT;
	if(Input::getKeyState(XD_KEY_LCONTROL)) layer = TERRAIN_LAYER_BACK;
	if(Input::getKeyState(XD_LMB))
	{
		int x = floor(m_world->getCamera()->getInputPosition().x/BLOCK_PXF), y = floor(m_world->getCamera()->getInputPosition().y/BLOCK_PXF);

		m_world->getTerrain()->setBlockAt(x, y, m_block, layer);

		if(Server::getInstance())
		{
			RakNet::BitStream bitStream;
			bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
			bitStream.Write(x);
			bitStream.Write(y);
			bitStream.Write(m_block);
			bitStream.Write(layer);
			Server::getInstance()->sendPacket(&bitStream);
		}
	}
	else if(Input::getKeyState(XD_RMB))
	{
		m_world->getTerrain()->setBlockAt(floor((m_world->getCamera()->getPosition().x + Input::getPosition().x)/BLOCK_PXF), floor((m_world->getCamera()->getPosition().y + Input::getPosition().y)/BLOCK_PXF), BLOCK_EMPTY, layer);
	}*/
}

void Debug::draw(SpriteBatch *spriteBatch)
{
	if(!m_enabled) return;
	
	setVariable("Chunks", util::intToStr(m_world->getTerrain()->getChunkLoader()->m_chunks.size()) + " / " + util::intToStr(m_world->getTerrain()->getChunkLoader()->m_optimalChunkCount));
	//setVariable("Time", util::intToStr(m_timeOfDay->getHour()) + ":" + util::intToStr(m_timeOfDay->getMinute()));
	Vector2 center = m_world->getCamera()->getCenter();
	setVariable("Camera", util::floatToStr(center.x) + ", " + util::floatToStr(center.y));
	setVariable("Zoom", util::intToStr(m_world->getCamera()->getZoomLevel() * 100) + "%");

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
	gfxContext.setProjectionMatrix(m_world->getCamera()->getProjectionMatrix());
	gfxContext.setTexture(nullptr);
	if(Input::getKeyState(XD_KEY_B))
	{
		int x0 = (int)floor(m_world->getCamera()->getX()/BLOCK_PXF);
		int y0 = (int)floor(m_world->getCamera()->getY()/BLOCK_PXF);
		int x1 = (int)floor((m_world->getCamera()->getX() + m_world->getCamera()->getWidth())/BLOCK_PXF);
		int y1 = (int)floor((m_world->getCamera()->getY() + m_world->getCamera()->getHeight())/BLOCK_PXF);

		for(int y = y0; y <= y1; ++y)
		{
			gfxContext.drawRectangle(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 127, 255));
		}

		for(int x = x0; x <= x1; ++x)
		{
			gfxContext.drawRectangle(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (y1 - y0 + 1) * BLOCK_PXF, Color(127, 127, 127, 255));
		}
	}

	// Draw chunk grid
	Vector2 position = m_world->getCamera()->getPosition();
	Vector2 size = m_world->getCamera()->getSize();
	int x0 = (int)floor(position.x/CHUNK_PXF);
	int y0 = (int)floor(position.y/CHUNK_PXF);
	int x1 = (int)floor((position.x+size.x)/CHUNK_PXF);
	int y1 = (int)floor((position.y+size.y)/CHUNK_PXF);

	for(int y = y0; y <= y1; ++y)
	{
		gfxContext.drawRectangle(x0 * CHUNK_PXF, y * CHUNK_PXF, (x1 - x0 + 1) * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(0, 0, 0, 255));
	}

	for(int x = x0; x <= x1; ++x)
	{
		gfxContext.drawRectangle(x * CHUNK_PXF, y0 * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (y1 - y0 + 1) * CHUNK_PXF, Color(0, 0, 0, 255));
	}

	if(m_debugChunkLoader)
	{
		// Draw current view
		Vector2 position = m_world->getCamera()->getCenter() - Window::getSize()*0.5f;
		Vector2 size = Window::getSize();
		gfxContext.drawRectangle(position.x, position.y, 1.0f / m_world->getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		gfxContext.drawRectangle((position.x+size.x), position.y, 1.0f / m_world->getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		gfxContext.drawRectangle(position.x, position.y, size.x, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 255, 255));
		gfxContext.drawRectangle(position.x, (position.y+size.y), size.x, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 255, 255));

		// Draw active area
		ChunkLoader::ChunkArea activeArea = m_world->getTerrain()->getChunkLoader()->getActiveArea();
		gfxContext.drawRectangle(activeArea.x0 * CHUNK_PX, activeArea.y0 * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), (activeArea.y1 - activeArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle((activeArea.x1+1) * CHUNK_PX, activeArea.y0 * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), (activeArea.y1 - activeArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle(activeArea.x0 * CHUNK_PX, activeArea.y0 * CHUNK_PX, (activeArea.x1 - activeArea.x0 + 1) * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));
		gfxContext.drawRectangle(activeArea.x0 * CHUNK_PX, (activeArea.y1+1) * CHUNK_PX, (activeArea.x1 - activeArea.x0 + 1) * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));

		// Draw load area
		ChunkLoader::ChunkArea loadArea = m_world->getTerrain()->getChunkLoader()->getLoadArea();
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, loadArea.y0 * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle((loadArea.x1+1) * CHUNK_PX, loadArea.y0 * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, loadArea.y0 * CHUNK_PX, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, (loadArea.y1+1) * CHUNK_PX, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));

		for(int y = y0; y <= y1; ++y)
		{
			for(int x = x0; x <= x1; ++x)
			{
				if(!m_world->getTerrain()->getChunkLoader()->isChunkLoadedAt(x, y))
				{
					gfxContext.drawRectangle(x * CHUNK_PX, y * CHUNK_PX, CHUNK_PX, CHUNK_PX, Color(255, 0, 0, 127));
				}
				else if(m_world->getTerrain()->getChunkLoader()->getChunkAt(x, y).isDirty(TERRAIN_LAYER_MIDDLE))
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
		spriteBatch->drawSprite(Sprite(m_world->getLighting()->m_lightingRenderTarget->getTexture(), Rect(0.0f, 128.0f, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_world->getLighting()->m_lightingPass0->getTexture(), Rect(0.0f, 128.0f*2, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_world->getLighting()->m_lightingPass1->getTexture(), Rect(0.0f, 128.0f*3, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_world->getLighting()->m_lightingPass2->getTexture(), Rect(0.0f, 128.0f*4, 256.0f, 128.0f)));
		spriteBatch->end();
		gfxContext.enable(GraphicsContext::BLEND);
	}

	// Show light sources as light bulbs
	spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_world->getCamera()->getProjectionMatrix()));
	for(list<LightSource*>::iterator itr = m_world->getLighting()->m_lightSources.begin(); itr != m_world->getLighting()->m_lightSources.end(); ++itr)
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