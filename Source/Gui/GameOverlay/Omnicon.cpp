#include "Omnicon.h"
#include "Entities/Player.h"

Omnicon::Omnicon(Scene *scene, Player *player, UiObject *parent) :
	UiObject(scene, parent),
	m_visible(false),
	m_player(player),
	m_omnicon(ResourceManager::get<Texture2D>(":/Sprites/Gui/GameOverlay/Omnicon.png"))
{
	m_omnicon.setSize(640, 400);
	m_omnicon.setPosition(0,0);

	Input::bind(XD_KEY_B, bind(&Omnicon::toggle, this));
}

void Omnicon::toggle()
{
	m_visible = !m_visible;
}

void Omnicon::update(const float delta)
{
	if(!m_visible && !m_player->isCrafting()) return;
}

void Omnicon::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(!m_visible && !m_player->isCrafting()) return;

	spriteBatch->drawSprite(m_omnicon);
}