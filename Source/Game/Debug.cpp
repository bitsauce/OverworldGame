#include "BitStream.h"

#include "Debug.h"
#include "Constants.h"
#include "World/World.h"
#include "Entities/Player.h"
#include "Blocks/BlockData.h"
#include "BlockEntities/BlockEntityData.h"
#include "Networking/Server.h"
#include "Entities/Mobs/Zombie.h"
#include "Entities/DebugLight.h"
#include "Game/States/GameState.h"
#include "Game/Game.h"
#include "Gui/GameOverlay/GameOverlay.h"
#include "Gui/ColorPicker.h"

#define DEBUG_FUNCTIONS_STRING \
	"F1: Toggle debug\n" \
	"F2: Toggle lighting\n" \
	"F3: Toggle overlays\n" \
	"F4: Toggle block painter\n" \
	"F5: Show chunk loader info\n" \
	"F6: Show lighting info\n" \
	"F7: Toggle light painter\n" \
	"F8: Spawn menu\n" \
	"F9: [No function]\n" \
	"F10: Detach/attach camera\n" \
	"F11: Stress-test block entitites\n" \
	"F12: [No function]"

Debug::Debug(OverworldGame *game) :
	m_game(game),
	m_world(game->getWorld()),
	m_block(BlockData::s_idToData.cbegin()),
	m_enabled(false),
	m_debugChunkLoader(false),
	m_debugMode(DEBUG_MODE_DEFAULT),
	m_font(Resource<Font>("Fonts/Debug")),
	m_drawCircleShader(Resource<Shader>("Shaders/Draw_Circle")),
	m_blockPainterTexture(new Texture2D()),
	m_colorPicker(0),
	m_newPointlight(0),
	m_selectedLight(0),
	m_lmbState(false),
	m_moveCount(0)
{
	// Make default block not the empty block
	m_block++;

	// Set font color
	m_font->setColor(Color(255, 255, 255, 255));
}

void Debug::debugFunction(InputEvent *_e)
{
	KeyEvent *e = static_cast<KeyEvent*>(_e);
	// Debug functions only called when debug enabled
	if(e->getType() != KeyEvent::DOWN || (!m_enabled && e->getKeycode() != SAUCE_KEY_F1))
	{
		return;
	}

	switch(e->getKeycode())
	{
		// Toggle debug
		case SAUCE_KEY_F1: toggle(); break;

		// Toggle lighting
		case SAUCE_KEY_F2:
		{
			m_world->getLighting()->m_enabled = !m_world->getLighting()->m_enabled;
		}
		break;

		// Toggle overlay
		case SAUCE_KEY_F3:
		{
			m_game->getGameOverlay()->m_active = !m_game->getGameOverlay()->m_active;
		}
		break;

		// Toggle block painter
		case SAUCE_KEY_F4:
		{
			m_debugMode = m_debugMode != DEBUG_MODE_BLOCK_PAINTER ? DEBUG_MODE_BLOCK_PAINTER : DEBUG_MODE_DEFAULT;
		}
		break;

		// Toggle chunk loader info
		case SAUCE_KEY_F5:
		{
			m_debugChunkLoader = !m_debugChunkLoader;
			m_world->getTerrain()->getChunkManager()->m_applyZoom = !m_world->getTerrain()->getChunkManager()->m_applyZoom;
		}
		break;

		// Toggle lighting info
		case SAUCE_KEY_F6:
		{
			m_debugMode = m_debugMode != DEBUG_MODE_LIGHT ? DEBUG_MODE_LIGHT : DEBUG_MODE_DEFAULT;
		}
		break;

		case SAUCE_KEY_F7:
		{
			m_debugMode = m_debugMode != DEBUG_MODE_LIGHT_PAINTER ? DEBUG_MODE_LIGHT_PAINTER : DEBUG_MODE_DEFAULT;
			if(m_debugMode == DEBUG_MODE_LIGHT_PAINTER)
			{
				m_game->getGameOverlay()->m_active = false;
				m_colorPicker = new ColorPicker(m_game->getCanvas());
				m_colorPicker->setOrigin(0.0f, 1.0f);
				m_colorPicker->setAnchor(0.05f, 0.95f);
				m_colorPicker->setSize(Vector2F(128.0f) / m_game->getCanvas()->getDrawSize());
			}
			else
			{
				m_game->getGameOverlay()->m_active = true;
				delete m_colorPicker;
			}
		}
		break;

		// Show spawn menu
		case SAUCE_KEY_F8:
		{
		}
		break;

		case SAUCE_KEY_F9:
		{
		}
		break;

		// Attach/detach camera
		case SAUCE_KEY_F10:
		{
			Camera *camera = m_world->getCamera();
			if(camera->getTargetEntity())
			{
				// Detach camera
				camera->setTargetEntity(nullptr);
			}
			else
			{
				// If we hold SHIFT, we move the player to the camera position
				if((e->getModifiers() & KeyEvent::SHIFT) != 0)
				{
					m_world->getLocalPlayer()->setPosition(camera->getCenter(0.0f));
				}

				// Attach camera to local player
				camera->setTargetEntity(m_world->getLocalPlayer());
			}
		}
		break;

		case SAUCE_KEY_F11:
		{
			break;
			ChunkManager::ChunkArea area = m_world->getTerrain()->getChunkManager()->getActiveArea();
			for(int y = area.y0 * CHUNK_BLOCKS; y <= area.y1 * CHUNK_BLOCKS; ++y)
			{
				for(int x = area.x0 * CHUNK_BLOCKS; x <= area.x1 * CHUNK_BLOCKS; ++x)
				{
					BlockEntityData::get(BLOCK_ENTITY_TORCH)->create(x, y);
				}
			}
		}
		break;

		case SAUCE_KEY_F12:
		{
		}
		break;
	}
}

void Debug::onTick(TickEvent *e)
{
	if(!m_enabled) return;

	switch(m_debugMode)
	{
		case DEBUG_MODE_BLOCK_PAINTER:
		{
			// Block painting
			WorldLayer layer = WORLD_LAYER_MIDDLE;
			if(m_game->getInputManager()->getKeyState(SAUCE_KEY_LSHIFT)) layer = WORLD_LAYER_FRONT;
			if(m_game->getInputManager()->getKeyState(SAUCE_KEY_LCTRL)) layer = WORLD_LAYER_BACK;
			if(m_game->getInputManager()->getKeyState(SAUCE_MOUSE_BUTTON_LEFT) || m_game->getInputManager()->getKeyState(SAUCE_MOUSE_BUTTON_RIGHT))
			{
				m_world->getTerrain()->setBlockAt(
					(int) floor(m_world->getCamera()->getInputPosition().x / BLOCK_PXF),
					(int) floor(m_world->getCamera()->getInputPosition().y / BLOCK_PXF),
					layer,
					m_game->getInputManager()->getKeyState(SAUCE_MOUSE_BUTTON_LEFT) ? m_block->second : BlockData::get(0),
					true);
			}
		}
		break;

		case DEBUG_MODE_LIGHT_PAINTER:
		{
			// Update active pointlight
			if(m_selectedLight)
			{
				m_selectedLight->getPointlight()->setColor(m_colorPicker->getSelectedColor());
			}
		}
		break;
	}
}

void Debug::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

	GraphicsContext *context = e->getGraphicsContext();
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	spriteBatch->end();
	spriteBatch->begin(context);

	// Set FPS output
	addVariable("FPS", util::intToStr((int) m_game->getFPS()));

	switch(m_debugMode)
	{
		case DEBUG_MODE_DEFAULT:
		{
			Vector2I center = m_world->getCamera()->getCenter(e->getAlpha());
			Vector2F inputPosition = m_world->getCamera()->getInputPosition();

			addVariable("Chunks", util::intToStr(m_world->getTerrain()->getChunkManager()->m_chunks.size()) + " / " + util::intToStr(m_world->getTerrain()->getChunkManager()->m_optimalChunkCount));

			{
				const int chunkX = (int) floor(inputPosition.x / CHUNK_PXF);
				const int chunkY = (int) floor(inputPosition.y / CHUNK_PXF);
				addVariable("Chunk Under Cursor", util::intToStr(chunkX) + ", " + util::intToStr(chunkY));
				addVariable("# Entities in Chunk Under Cursor", util::intToStr(m_world->getTerrain()->getChunkManager()->getChunkAt(chunkX, chunkY, true)->m_entities.size()));
			}

			addVariable("Camera", util::floatToStr(center.x) + ", " + util::floatToStr(center.y));
			addVariable("Zoom", util::intToStr(int(m_world->getCamera()->getZoomLevel() * 100)) + "%");
			{
				const BlockData *blocks[WORLD_LAYER_COUNT];
				const int blockX = (int) floor(inputPosition.x / BLOCK_PXF);
				const int blockY = (int) floor(inputPosition.y / BLOCK_PXF);

				blocks[WORLD_LAYER_BACK] = m_world->getTerrain()->getBlockAt(blockX, blockY, WORLD_LAYER_BACK);
				blocks[WORLD_LAYER_MIDDLE] = m_world->getTerrain()->getBlockAt(blockX, blockY, WORLD_LAYER_MIDDLE);
				blocks[WORLD_LAYER_FRONT] = m_world->getTerrain()->getBlockAt(blockX, blockY, WORLD_LAYER_FRONT);

				stringstream ss;
				if(blocks[WORLD_LAYER_BACK]->getID() == 0 && blocks[WORLD_LAYER_MIDDLE]->getID() == 0 && blocks[WORLD_LAYER_FRONT]->getID() == 0)
				{
					ss << "None";
				}
				else
				{
					if(blocks[WORLD_LAYER_BACK]->getID() != 0)
					{
						ss << "Back: " << blocks[WORLD_LAYER_BACK]->getName() << " ";
					}
					if(blocks[WORLD_LAYER_MIDDLE]->getID() != 0)
					{
						ss << "Middle: " << blocks[WORLD_LAYER_MIDDLE]->getName() << " ";
					}
					if(blocks[WORLD_LAYER_FRONT]->getID() != 0)
					{
						ss << "Front: " << blocks[WORLD_LAYER_FRONT]->getName() << " ";
					}
					ss << "at " << blockX << ", " << blockY;
				}
				addVariable("Blocks Under Cursor", ss.str());
			}

			{
				BlockEntity *blockEntity = m_world->getTerrain()->getBlockEntityAt((int) floor(inputPosition.x / BLOCK_PXF), (int) floor(inputPosition.y / BLOCK_PXF), WORLD_LAYER_MIDDLE);
				const string name = blockEntity ? blockEntity->getData()->getName() : "NULL";
				addVariable("Block Entity Under Cursor", name + " at " + util::intToStr((int) floor(inputPosition.x / BLOCK_PXF)) + ", " + util::intToStr((int) floor(inputPosition.y / BLOCK_PXF)));
			}

			string hourStr, minStr;
			{
				int hour = m_world->getTimeOfDay()->getHour();
				hourStr = hour < 10 ? ("0" + util::intToStr(hour)) : util::intToStr(hour);
				int min = m_world->getTimeOfDay()->getMinute();
				minStr = min < 10 ? ("0" + util::intToStr(min)) : util::intToStr(min);
			}
			addVariable("Time", hourStr + ":" + minStr);

			if(m_game->getWorld()->getLocalPlayer())
			{
				Player *player = m_game->getWorld()->getLocalPlayer();
				Controller *controller = player->getController();
				addVariable("Velocity", player->getVelocity().toString());
				addVariable("Move dir", util::intToStr(controller->getInputState(Controller::INPUT_MOVE_RIGHT) - controller->getInputState(Controller::INPUT_MOVE_LEFT)));
				addVariable("Running", util::intToStr(controller->getInputState(Controller::INPUT_RUN)));
			}
		}
		break;
	}

	// Draw top-left debug info
	string drawString;
	for(list<pair<string, string>>::iterator itr = m_variables.begin(); itr != m_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}
	m_font->draw(spriteBatch, Vector2F(0.0f), drawString);
	m_font->draw(spriteBatch, Vector2F((float) context->getWidth(), 0.0f), DEBUG_FUNCTIONS_STRING, FONT_ALIGN_RIGHT);
	m_variables.clear();

	// Block painter
	switch(m_debugMode)
	{
		case DEBUG_MODE_BLOCK_PAINTER:
		{
			m_blockPainterTexture->updatePixmap(m_block->second->getPixmap());
			spriteBatch->drawText(Vector2F(5.0f, context->getHeight() - 48.0f), "Current block:   (" + util::intToStr(m_block->second->getID()) + ")\n" + "Current layer: " + (m_game->getInputManager()->getKeyState(SAUCE_KEY_LCTRL) ? "BACK" : (m_game->getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) ? "FRONT" : "SCENE")), m_font.get());
			Sprite blockSprite(m_blockPainterTexture, RectF(m_font->getStringWidth("Current block:"), context->getHeight() - 60.0f, 32.0f, 32.0f), Vector2F(0.0f, 0.0f), 0.0f, TextureRegion(0.0f, 1.0f / 3.0f, 1.0f, 1.0f));
			spriteBatch->drawSprite(blockSprite);
		}
		break;
	}

	// Draw block grid
	context->setTransformationMatrix(m_world->getCamera()->getTransformationMatrix(e->getAlpha()));
	context->setTexture(nullptr);
	Vector2F position = m_world->getCamera()->getPosition();
	Vector2F size = m_world->getCamera()->getSize();
	if(m_game->getInputManager()->getKeyState(SAUCE_KEY_K))
	{
		int x0 = (int) floor(position.x / BLOCK_PXF);
		int y0 = (int) floor(position.y / BLOCK_PXF);
		int x1 = (int) floor((position.x + size.x) / BLOCK_PXF);
		int y1 = (int) floor((position.y + size.y) / BLOCK_PXF);

		for(int y = y0; y <= y1; ++y)
		{
			context->drawRectangle(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(80, 80, 80, 80));
		}

		for(int x = x0; x <= x1; ++x)
		{
			context->drawRectangle(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (y1 - y0 + 1) * BLOCK_PXF, Color(80, 80, 80, 80));
		}
	}

	// Draw chunk grid
	int x0 = (int) floor(position.x / CHUNK_PXF);
	int y0 = (int) floor(position.y / CHUNK_PXF);
	int x1 = (int) floor((position.x + size.x) / CHUNK_PXF);
	int y1 = (int) floor((position.y + size.y) / CHUNK_PXF);

	for(int y = y0; y <= y1; ++y)
	{
		context->drawRectangle(x0 * CHUNK_PXF, y * CHUNK_PXF, (x1 - x0 + 1) * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(0, 0, 0, 80));
	}

	for(int x = x0; x <= x1; ++x)
	{
		context->drawRectangle(x * CHUNK_PXF, y0 * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (y1 - y0 + 1) * CHUNK_PXF, Color(0, 0, 0, 80));
	}

	// Draw debug chunks
	if(m_debugChunkLoader)
	{
		// Draw current view
		Vector2F position = m_world->getCamera()->getCenter(e->getAlpha()) - context->getSize() * 0.5f;
		Vector2F size = context->getSize();
		context->drawRectangle(position.x, position.y, 1.0f / m_world->getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		context->drawRectangle((position.x + size.x), position.y, 1.0f / m_world->getCamera()->getZoomLevel(), size.y, Color(127, 127, 255, 255));
		context->drawRectangle(position.x, position.y, size.x, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 255, 255));
		context->drawRectangle(position.x, (position.y + size.y), size.x, 1.0f / m_world->getCamera()->getZoomLevel(), Color(127, 127, 255, 255));

		// Draw load area
		ChunkManager::ChunkArea loadArea = m_world->getTerrain()->getChunkManager()->getLoadingArea();
		context->drawRectangle(loadArea.x0 * CHUNK_PXF, loadArea.y0 * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PXF, Color(255, 255, 255, 255));
		context->drawRectangle((loadArea.x1 + 1) * CHUNK_PXF, loadArea.y0 * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), (loadArea.y1 - loadArea.y0 + 1) * CHUNK_PXF, Color(255, 255, 255, 255));
		context->drawRectangle(loadArea.x0 * CHUNK_PXF, loadArea.y0 * CHUNK_PXF, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));
		context->drawRectangle(loadArea.x0 * CHUNK_PXF, (loadArea.y1 + 1) * CHUNK_PXF, (loadArea.x1 - loadArea.x0 + 1) * CHUNK_PXF, 1.0f / m_world->getCamera()->getZoomLevel(), Color(255, 255, 255, 255));

		// Draw chunk status
		for(int y = y0; y <= y1; ++y)
		{
			for(int x = x0; x <= x1; ++x)
			{
				// DARK BLUE: No blocks loaded
				// LIGHT BLUE: Blocks loaded in memory, but not rendered to the global render target
				// RED: Blocks rendered to the global render target, but not sorted
				// BLANK: Blocks rendered and sorted.
				if(!m_world->getTerrain()->getChunkManager()->isChunkLoadedAt(x, y))
				{
					context->drawRectangle(x * CHUNK_PXF, y * CHUNK_PXF, CHUNK_PXF, CHUNK_PXF, Color(0, 100, 170, 127));
				}
				else if(!m_world->getTerrain()->getChunkManager()->getChunkAt(x, y, true)->isAttached())
				{
					context->drawRectangle(x * CHUNK_PXF, y * CHUNK_PXF, CHUNK_PXF, CHUNK_PXF, Color(0, 160, 230, 127));
				}
				else if(!m_world->getTerrain()->getChunkManager()->getChunkAt(x, y, true)->isSorted())
				{
					context->drawRectangle(x * CHUNK_PXF, y * CHUNK_PXF, CHUNK_PXF, CHUNK_PXF, Color(230, 30, 30, 127));
				}
			}
		}
	}

	// Debug lighting
	if(m_debugMode == DEBUG_MODE_LIGHT_PAINTER)
	{
		// Show light sources
		for(list<DebugPointlight*>::iterator itr = DebugPointlight::s_pointlights.begin(); itr != DebugPointlight::s_pointlights.end(); ++itr)
		{
			LightSource *light = (*itr)->getPointlight();
			context->setShader(m_drawCircleShader);
			m_drawCircleShader->setUniformColor("u_Color", light->getColor());
			m_drawCircleShader->setUniform1ui("u_DrawOutline", m_selectedLight && light == m_selectedLight->getPointlight());
			context->drawRectangle((light->getPosition() - Vector2F(light->getRadius())) * BLOCK_PXF, Vector2F(light->getRadius() * 2.0f) * BLOCK_PXF);
		}
		context->setShader(0);
	}

	if(m_debugMode == DEBUG_MODE_LIGHT)
	{
		// Show lighting passes
		context->setTransformationMatrix(Matrix4());
		context->drawRectangle(0.0f, 24.0f, 256.0f, 130.0f * 3, Color(0, 255, 0, 255));
		context->disable(GraphicsContext::BLEND);
		context->setTexture(m_world->getLighting()->m_staticLightingRenderTarget->getTexture()); context->drawRectangle(0.0f, 24.0f + 130.0f * 0, 256.0f, 128.0f); m_font->draw(spriteBatch, 0.0f, 24.0f + 130.0f * 0, "Static");
		context->setTexture(m_world->getLighting()->m_dynamicLightingRenderTarget->getTexture()); context->drawRectangle(0.0f, 24.0f + 130.0f * 1, 256.0f, 128.0f); m_font->draw(spriteBatch, 0.0f, 24.0f + 130.0f * 1, "Dynamic");
		context->setTexture(m_world->getLighting()->m_dynamicLightingBlurH->getTexture()); context->drawRectangle(0.0f, 24.0f + 130.0f * 2, 256.0f, 128.0f); m_font->draw(spriteBatch, 0.0f, 24.0f + 130.0f * 2, "BlurH");
		context->setTexture(m_world->getLighting()->m_dynamicLightingBlurV->getTexture()); context->drawRectangle(0.0f, 24.0f + 130.0f * 3, 256.0f, 128.0f); m_font->draw(spriteBatch, 0.0f, 24.0f + 130.0f * 3, "Final");
		context->enable(GraphicsContext::BLEND);
	}

	if(m_debugMode == DEBUG_MODE_LIGHT || m_debugMode == DEBUG_MODE_LIGHT_PAINTER)
	{
		for(list<LightSource*>::iterator itr = m_world->getLighting()->m_lightSources.begin(); itr != m_world->getLighting()->m_lightSources.end(); ++itr)
		{
			LightSource *light = *itr;
			context->drawCircle(light->getPosition() * BLOCK_PXF, 5.0f, 12, light->getColor());
		}
	}
}

void Debug::onMouseEvent(MouseEvent *e)
{
	switch(m_debugMode)
	{
		case DEBUG_MODE_LIGHT_PAINTER:
		{
			if(m_colorPicker->getDrawRect().contains(e->getPosition()))
			{
				break;
			}

			switch(e->getType())
			{
				case MouseEvent::DOWN:
				{
					// Check for left mouse button
					if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT)
					{
						// Select a light source
						for(list<DebugPointlight*>::iterator itr = DebugPointlight::s_pointlights.begin(); itr != DebugPointlight::s_pointlights.end(); ++itr)
						{
							DebugPointlight *lightEntity = *itr;
							LightSource *light = lightEntity->getPointlight();
							if(RectF(light->getPosition() - Vector2F(light->getRadius()), Vector2F(light->getRadius() * 2.0f)).contains(Vector2F(m_game->getWorld()->getCamera()->getInputPosition() / BLOCK_PXF)))
							{
								m_selectedLight = lightEntity;
								m_colorPicker->setSelectedColor(m_selectedLight->getPointlight()->getColor());
								m_moveCount = 0;
								m_lightDragOffset = m_selectedLight->getPosition() - m_game->getWorld()->getCamera()->getInputPosition();
								break;
							}
							m_selectedLight = 0;
						}
						m_lmbState = true;
					}
					else if(e->getButton() == SAUCE_MOUSE_BUTTON_RIGHT)
					{
						// Create new point light
						/*m_newPointlight = new DebugPointlight(m_world);
						m_newPointlight->setPosition(Vector2F(m_world->getCamera()->getInputPosition()));
						m_newPointlight->getPointlight()->setColor(m_colorPicker->getSelectedColor());
						m_selectedLight = m_newPointlight;*/
					}
				}
				break;

				case MouseEvent::MOVE:
				{
					if(m_newPointlight)
					{
						// Set new light radius
						Pointlight *pointlight = m_newPointlight->getPointlight();
						pointlight->setRadius((pointlight->getPosition() - Vector2F(m_game->getWorld()->getCamera()->getInputPosition()) / BLOCK_PXF).length());
					}
					else if(m_lmbState && m_selectedLight)
					{
						// Drag-move selected light
						if(m_moveCount++ > 2)
						{
							m_selectedLight->setPosition(Vector2F(m_game->getWorld()->getCamera()->getInputPosition()) + m_lightDragOffset);
						}
					}
				}
				break;

				case MouseEvent::UP:
				{
					if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT)
					{
						m_lmbState = false;
					}
					else if(e->getButton() == SAUCE_MOUSE_BUTTON_RIGHT)
					{
						m_newPointlight = 0;
					}
				}
				break;

				case MouseEvent::WHEEL:
				{
					// Change selected light radius
					if(m_selectedLight)
					{
						float newRadius = m_selectedLight->getPointlight()->getRadius() + (e->getWheelY() > 0 ? 1.0f : -1.0f);
						m_selectedLight->getPointlight()->setRadius(max(newRadius, 0.0f));
					}
				}
				break;
			}
		}
		break;
	}
}

void Debug::onKeyEvent(KeyEvent *e)
{
	switch(m_debugMode)
	{
		case DEBUG_MODE_LIGHT_PAINTER:
		{
			if(e->getType() == KeyEvent::DOWN)
			{
				if(m_selectedLight && e->getKeycode() == SAUCE_KEY_DELETE)
				{
					delete m_selectedLight;
					m_selectedLight = 0;
				}
			}
		}
		break;
	}
}

void Debug::toggle()
{
	m_enabled = !m_enabled;
}

void Debug::nextBlock(InputEvent *e)
{
	if(m_enabled && e->getType() == KeyEvent::DOWN)
	{
		m_block++;
		if(m_block == BlockData::s_idToData.cend())
		{
			m_block = BlockData::s_idToData.cbegin();
		}
	}
}

void Debug::prevBlock(InputEvent *e)
{
	if(m_enabled && e->getType() == KeyEvent::DOWN)
	{
		if(m_block == BlockData::s_idToData.cbegin())
		{
			m_block = BlockData::s_idToData.cend();
		}
		m_block--;
	}
}

void Debug::addVariable(const string &name, const string &value)
{
	m_variables.push_back(make_pair(name, value));
}