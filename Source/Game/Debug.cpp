#include "BitStream.h"

#include "Debug.h"
#include "Constants.h"
#include "World/World.h"
#include "World/Camera.h"
#include "Entities/Dynamic/Player.h"
#include "Blocks/BlockData.h"
#include "Networking/Server.h"
#include "Entities/Dynamic/Mobs/Zombie.h"
#include "Game/GameStates/GameState.h"
#include "Game/Game.h"
#include "Gui/GameOverlay/GameOverlay.h"

#define DEBUG_FUNCTIONS_STRING "F1: Toggle debug\nF2: Toggle lighting\nF3: Toggle overlays\nF4: Toggle block painter\nF5: Show chunk loader info\nF6: Show lighting info\nF7: Set time\nF8: Spawn menu\nF9: Multiplayer menu\nF10: Detach/attach camera\nF11: [No func]\nF12: [No func]"

Debug::Debug(OverworldGame *game) :
	m_game(game),
	m_world(game->getWorld()),
	m_block(BLOCK_GRASS),
	m_enabled(false),
	m_debugChunkLoader(false),
	m_debugLighting(false),
	m_blockPainterEnabled(false),
	m_font(ResourceManager::get<Font>(UI_DEBUG_FONT)),
	m_variables(),
	m_bulbSprite(ResourceManager::get<Texture2D>(":/Sprites/Debug/Icon_Bulb.png"))
{
	// Set font color
	m_font->setColor(Color(255, 255, 255, 255));

	m_bulbSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Debug::debugFunction(int action, const int i)
{
	// Debug functions only called when debug enabled
	if(action != GLFW_PRESS || (!m_enabled && i != 1))
	{
		return;
	}

	switch(i)
	{
		// Toggle debug
		case 1: toggle(); break;

		// Toggle lighting
		case 2:
		{
			m_world->getLighting()->m_enabled = !m_world->getLighting()->m_enabled;
		}
		break;

		// Toggle overlay
		case 3:
		{
			m_game->getGameOverlay()->m_hidden = !m_game->getGameOverlay()->m_hidden;
		}
		break;

		// Toggle block painter
		case 4:
		{
			m_blockPainterEnabled = !m_blockPainterEnabled;
		}
		break;

		// Toggle chunk loader info
		case 5:
		{
			m_debugChunkLoader = !m_debugChunkLoader;
			m_world->getTerrain()->getChunkLoader()->m_applyZoom = !m_world->getTerrain()->getChunkLoader()->m_applyZoom;
		}
		break;

		// Toggle lighting info
		case 6:
		{
			m_debugLighting = !m_debugLighting;
		}
		break;

		// Set time
		case 7:
		{
			m_world->getTimeOfDay()->setTime(m_world->getTimeOfDay()->getTime() + (Input::getKeyState(XD_KEY_LEFT_SHIFT) == GLFW_PRESS ? -100 : 100));
		}
		break;

		// Show spawn menu
		case 8:
		{
			// Spawn light
			new Pointlight(m_world->getLighting(), m_world->getCamera()->getInputPosition() / BLOCK_PXF, 20, Color((uchar) m_random.nextInt(255), (uchar) m_random.nextInt(255), (uchar) m_random.nextInt(255)));
			// Spawn zombie

			// Create zombie
			/*RakNet::BitStream bitStream;
			bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
			bitStream.Write(ENTITY_ZOMBIE);
			((Server*)Connection::getInstance())->getRakPeer()->SendLoopback((const char*) bitStream.GetData(), bitStream.GetNumberOfBytesUsed());*/
		}
		break;

		// Multiplayer menu
		case 9:
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

		// Attach/detach camera
		case 10:
		{
			Camera *camera = m_world->getCamera();
			if(camera->getTargetEntity())
			{
				// Detach camera
				camera->setTargetEntity(nullptr);
			}
			else
			{
				// Attach camera to local player
				camera->setTargetEntity(m_world->getLocalPlayer());
			}
		}
		break;

		case 11:
		{
			m_world->getCamera()->lookAt(m_world->getCamera()->getInputPosition());
		}
		break;

		case 12:
		{
		}
		break;
	}
}

void Debug::update(const float delta)
{
	if(!m_enabled) return;

	if(m_blockPainterEnabled)
	{
		// Block painting
		TerrainLayer layer = TERRAIN_LAYER_MIDDLE;
		if(Input::getKeyState(XD_KEY_LEFT_SHIFT) == GLFW_PRESS) layer = TERRAIN_LAYER_FRONT;
		if(Input::getKeyState(XD_KEY_LEFT_CONTROL) == GLFW_PRESS) layer = TERRAIN_LAYER_BACK;
		if(Input::getKeyState(XD_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			int x = (int) floor(m_world->getCamera()->getInputPosition().x / BLOCK_PXF), y = (int) floor(m_world->getCamera()->getInputPosition().y / BLOCK_PXF);

			m_world->getTerrain()->setBlockAt(x, y, m_block, layer);
		}
		else if(Input::getKeyState(XD_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			m_world->getTerrain()->setBlockAt(floor((m_world->getCamera()->getPosition().x + Input::getPosition().x) / BLOCK_PXF), floor((m_world->getCamera()->getPosition().y + Input::getPosition().y) / BLOCK_PXF), BLOCK_EMPTY, layer);
		}
	}
}

void Debug::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(!m_enabled) return;

	// Set debug variables
	setVariable("Chunks", util::intToStr(m_world->getTerrain()->getChunkLoader()->m_chunks.size()) + " / " + util::intToStr(m_world->getTerrain()->getChunkLoader()->m_optimalChunkCount));
	Vector2 center = m_world->getCamera()->getCenter(alpha);
	Vector2 inputPosition = m_world->getCamera()->getInputPosition();
	setVariable("Camera", util::floatToStr(center.x) + ", " + util::floatToStr(center.y));
	setVariable("Zoom", util::intToStr(m_world->getCamera()->getZoomLevel() * 100) + "%");
	setVariable("Block Under Cursor", util::intToStr(m_world->getTerrain()->getBlockAt((int) floor(inputPosition.x / BLOCK_PXF), (int) floor(inputPosition.y / BLOCK_PXF), TERRAIN_LAYER_MIDDLE)) + " at " + util::intToStr((int) floor(inputPosition.x / BLOCK_PXF)) + ", " + util::intToStr((int) floor(inputPosition.y / BLOCK_PXF)));
	string hourStr, minStr;
	{
		int hour = m_world->getTimeOfDay()->getHour();
		hourStr = hour < 10 ? ("0" + util::intToStr(hour)) : util::intToStr(hour);
		int min = m_world->getTimeOfDay()->getMinute();
		minStr = min < 10 ? ("0" + util::intToStr(min)) : util::intToStr(min);
	}
	setVariable("Time", hourStr + ":" + minStr);

	// Draw top-left debug info
	string drawString;
	for(map<string, string>::iterator itr = m_variables.begin(); itr != m_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}
	m_font->draw(spriteBatch, Vector2(0.0f), drawString);

	m_font->draw(spriteBatch, Vector2(Window::getWidth(), 0.0f), DEBUG_FUNCTIONS_STRING, FONT_ALIGN_RIGHT);


	// Block painter
	if(m_blockPainterEnabled)
	{
		spriteBatch->drawText(Vector2(5.0f, Window::getSize().y - 48.0f), "Current block:   (" + util::intToStr(m_block) + ")\n" + "Current layer: " + (Input::getKeyState(XD_KEY_LEFT_CONTROL) == GLFW_PRESS ? "BACK" : (Input::getKeyState(XD_KEY_LEFT_SHIFT) == GLFW_PRESS ? "FRONT" : "SCENE")), m_font);
		Sprite blockSprite(BlockData::get(m_block).getTexture(), Rect(159.0f, Window::getSize().y - 50.0f, 32.0f, 32.0f), Vector2(0.0f, 0.0f), 0.0f, TextureRegion(0.0f, 0.0f, 1.0f, 2.0f / 3.0f));
		spriteBatch->drawSprite(blockSprite);
	}

	// Draw block grid
	GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setModelViewMatrix(m_world->getCamera()->getModelViewMatrix(alpha));
	gfxContext.setTexture(nullptr);
	Vector2 position = m_world->getCamera()->getPosition();
	Vector2 size = m_world->getCamera()->getSize();
	if(Input::getKeyState(XD_KEY_K) == GLFW_PRESS)
	{
		int x0 = (int) floor(position.x / BLOCK_PXF);
		int y0 = (int) floor(position.y / BLOCK_PXF);
		int x1 = (int) floor((position.x + size.x) / BLOCK_PXF);
		int y1 = (int) floor((position.y + size.y) / BLOCK_PXF);

		for(int y = y0; y <= y1; ++y)
		{
			gfxContext.drawRectangle(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(80, 80, 80, 80));
		}

		for(int x = x0; x <= x1; ++x)
		{
			gfxContext.drawRectangle(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (y1 - y0 + 1) * BLOCK_PXF, Color(80, 80, 80, 80));
		}
	}

	// Draw chunk grid
	int x0 = (int) floor(position.x / CHUNK_PXF);
	int y0 = (int) floor(position.y / CHUNK_PXF);
	int x1 = (int) floor((position.x + size.x) / CHUNK_PXF);
	int y1 = (int) floor((position.y + size.y) / CHUNK_PXF);

	for(int y = y0; y <= y1; ++y)
	{
		gfxContext.drawRectangle(x0 * CHUNK_PXF, y * CHUNK_PXF, (x1 - x0 + 1) * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(0, 0, 0, 80));
	}

	for(int x = x0; x <= x1; ++x)
	{
		gfxContext.drawRectangle(x * CHUNK_PXF, y0 * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (y1 - y0 + 1) * CHUNK_PXF, Color(0, 0, 0, 80));
	}

	// Draw debug chunks
	if(m_debugChunkLoader)
	{
		// Draw current view
		Vector2 position = m_world->getCamera()->getCenter(alpha) - Window::getSize()*0.5f;
		Vector2 size = Window::getSize();
		gfxContext.drawRectangle(position.x, position.y, 1.0f / m_world->getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		gfxContext.drawRectangle((position.x + size.x), position.y, 1.0f / m_world->getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		gfxContext.drawRectangle(position.x, position.y, size.x, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 255, 255));
		gfxContext.drawRectangle(position.x, (position.y + size.y), size.x, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 255, 255));

		// Draw load area
		ChunkLoader::ChunkArea loadArea = m_world->getTerrain()->getChunkLoader()->getLoadingArea();
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, loadArea.y0 * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle((loadArea.x1 + 1) * CHUNK_PX, loadArea.y0 * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PX, Color(255, 255, 255, 255));
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, loadArea.y0 * CHUNK_PX, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));
		gfxContext.drawRectangle(loadArea.x0 * CHUNK_PX, (loadArea.y1 + 1) * CHUNK_PX, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PX, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));

		// Draw chunk status
		for(int y = y0; y <= y1; ++y)
		{
			for(int x = x0; x <= x1; ++x)
			{
				if(!m_world->getTerrain()->getChunkLoader()->isChunkLoadedAt(x, y))
				{
					gfxContext.drawRectangle(x * CHUNK_PX, y * CHUNK_PX, CHUNK_PX, CHUNK_PX, Color(0, 100, 170, 127));
				}
				else if(!m_world->getTerrain()->getChunkLoader()->getChunkAt(x, y).isAttached())
				{
					gfxContext.drawRectangle(x * CHUNK_PX, y * CHUNK_PX, CHUNK_PX, CHUNK_PX, Color(0, 160, 230, 127));
				}
			}
		}
	}

	// Debug lighting
	if(m_debugLighting)
	{
		// Show lighting passes
		spriteBatch->flush();
		gfxContext.disable(GraphicsContext::BLEND);
		//spriteBatch->drawSprite(Sprite(m_world->getLighting()->m_lightingRenderTarget->getTexture(), Rect(0.0f, 128.0f, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_world->getTerrain()->getChunkLoader()->m_lightingPass0->getTexture(), Rect(0.0f, 128.0f * 2, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_world->getTerrain()->getChunkLoader()->m_lightingPass1->getTexture(), Rect(0.0f, 128.0f * 3, 256.0f, 128.0f)));
		spriteBatch->drawSprite(Sprite(m_world->getTerrain()->getChunkLoader()->m_lightingPass2->getTexture(), Rect(0.0f, 128.0f * 4, 256.0f, 128.0f)));
		spriteBatch->flush();
		gfxContext.enable(GraphicsContext::BLEND);

		// Show light sources as light bulbs // TODO: Replace with cricles
		spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_world->getCamera()->getModelViewMatrix(alpha)));
		for(list<LightSource*>::iterator itr = m_world->getLighting()->m_lightSources.begin(); itr != m_world->getLighting()->m_lightSources.end(); ++itr)
		{
			m_bulbSprite.setPosition((*itr)->getPosition() * BLOCK_PXF + Vector2(0.5f, 0.5f));
			spriteBatch->drawSprite(m_bulbSprite);
		}
	}
}

void Debug::toggle()
{
	m_enabled = !m_enabled;
}

void Debug::nextBlock(int action)
{
	if(action != GLFW_PRESS) return;
	if(!m_enabled) return;
	if((m_block = BlockID(m_block + 1)) >= BLOCK_COUNT)
	{
		m_block = BlockID(BLOCK_ENTITY + 1);
	}
}

void Debug::prevBlock(int action)
{
	if(action != GLFW_PRESS) return;
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