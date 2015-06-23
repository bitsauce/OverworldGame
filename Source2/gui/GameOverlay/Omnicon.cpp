#include "Omnicon.h"
#include "Game/Game.h"
//>REMOVE ME<
Omnicon::Omnicon(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_visible(false),
	m_gameOverlay(gameOverlay),
	m_bookSprite(ResourceManager::get<Texture2D>(":/Sprites/Gui/GameOverlay/Omnicon.png"))
{
	m_bookSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_bookSprite.setDepth(0.f);

	Input::bind(XD_KEY_B, bind(&Omnicon::toggle, this));
}

void Omnicon::toggle()
{
	m_visible = !m_visible;
}

void Omnicon::update(const float delta)
{
	if(!m_visible && !m_gameOverlay->isCrafting()) return;
}

void Omnicon::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(!m_visible && !m_gameOverlay->isCrafting()) return;

	spriteBatch->drawSprite(m_bookSprite);
}