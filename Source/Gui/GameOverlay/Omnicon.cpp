#include "Omnicon.h"
#include "Game/Game.h"
#include "GameOverlay.h"

Omnicon::Omnicon(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_visible(false),
	m_gameOverlay(gameOverlay),
	m_bookSprite(ResourceManager::get<Texture2D>(":/Sprites/Gui/GameOverlay/Omnicon.png"))
{
	m_bookSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_bookSprite.setDepth(0.f);
}

void Omnicon::toggle(int action)
{
	if(action != GLFW_PRESS) return;
	m_visible = !m_visible;
}

void Omnicon::onTick(TickEvent *e)
{
	if(!m_visible && !m_gameOverlay->isCrafting()) return;
}

void Omnicon::onDraw(DrawEvent *e)
{
	if(!m_visible && !m_gameOverlay->isCrafting()) return;

	spriteBatch->drawSprite(m_bookSprite);
}