#include <Sauce/Sauce.h>
#include "Generator.h"

using namespace sauce;

enum BlockID
{
	BLOCK_EMPTY,

	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_DIRT_BACK,

	BLOCK_OAK_WOOD,
	BLOCK_OAK_LEAVES,

	BLOCK_STONE,
	BLOCK_STONE_BACK,

	BLOCK_ANIM_TEST,
	BLOCK_TORCH_TEST,

	//BLOCK_SAND,
	//BLOCK_MUD,
	//BLOCK_,


	BLOCK_COUNT
};

class RandomNoiseGPU : public Game
{
	Generator *m_generator;
	Resource<RenderTarget2D> m_renderTarget;
	Resource<Font> m_font;
	SpriteBatch *m_spriteBatch;
	Random m_rand;


	Resource<Shader> m_drawBlocksShader;

	vector<Vector4F> m_blockColors;

	Resource<Texture2D> ttt;

public:
	RandomNoiseGPU() :
		Game("RandomNoiseGPU")
	{
	}

	void onStart(GameEvent *e)
	{
		m_generator = new Generator(this);

		getWindow()->setVSync(0);

		m_renderTarget = Resource<RenderTarget2D>(new RenderTarget2D(getWindow()->getWidth(), getWindow()->getHeight()));

		m_blockColors.resize(BLOCK_COUNT);
		m_blockColors[BLOCK_EMPTY] = Vector4F(0.78, 0.97, 0.96, 1.0);
		m_blockColors[BLOCK_GRASS] = Vector4F(0.1, 0.8, 0.2, 1.0);
		m_blockColors[BLOCK_DIRT] = Vector4F(0.3, 0.25, 0.19, 1.0);
		m_blockColors[BLOCK_DIRT_BACK] = Vector4F(m_blockColors[BLOCK_DIRT].getRGB() * 0.5, 1.0);
		m_blockColors[BLOCK_STONE] = Vector4F(0.5, 0.5, 0.5, 1.0);
		m_blockColors[BLOCK_STONE_BACK] = Vector4F(m_blockColors[BLOCK_STONE].getRGB() * 0.5, 1.0);

		m_drawBlocksShader = getResourceManager()->get<Shader>("DrawBlocks");
		m_drawBlocksShader->setUniform4f("u_BlockColors", (float*) m_blockColors.data());
		m_drawBlocksShader->setSampler2D("u_BlockTexture", m_renderTarget->getTexture());

		/*Pixmap ppp(1280, 720);
		uchar ixel[4] = { BLOCK_GRASS, BLOCK_GRASS, BLOCK_GRASS, BLOCK_GRASS };
		ppp.fill(ixel);
		ttt = Resource<Texture2D>(new Texture2D(ppp));
		m_drawBlocksShader->setSampler2D("u_BlockTexture", ttt);*/

		m_font = getResourceManager()->get<Font>("Font");
		m_spriteBatch = new SpriteBatch(getWindow()->getGraphicsContext());

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		m_generator->m_position.x += (getInputManager()->getKeyState(SAUCE_KEY_RIGHT) - getInputManager()->getKeyState(SAUCE_KEY_LEFT)) * 10.0f * ((getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) || getInputManager()->getKeyState(SAUCE_KEY_RSHIFT)) ? 5.0f : 1.0f) * m_generator->m_scale;
		m_generator->m_position.y += (getInputManager()->getKeyState(SAUCE_KEY_DOWN) - getInputManager()->getKeyState(SAUCE_KEY_UP)) * 10.0f * ((getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) || getInputManager()->getKeyState(SAUCE_KEY_RSHIFT)) ? 5.0f : 1.0f) * m_generator->m_scale;

		float prevScale = m_generator->m_scale;
		if(getInputManager()->getKeyState(SAUCE_KEY_PLUS))
		{
			m_generator->m_scale /= 1.1;
			m_generator->m_position -= Vector2F(getWindow()->getSize()) * 0.5f * (m_generator->m_scale - prevScale);
		}
		else if(getInputManager()->getKeyState(SAUCE_KEY_MINUS))
		{
			m_generator->m_scale *= 1.1;
			m_generator->m_position -= Vector2F(getWindow()->getSize()) * 0.5f * (m_generator->m_scale - prevScale);
		}

		if(getInputManager()->getKeyState(SAUCE_KEY_0))
		{
			m_generator->m_seed = m_rand.nextInt(1000);
		}

		m_generator->m_cliffingDelta += (getInputManager()->getKeyState(SAUCE_KEY_2) - getInputManager()->getKeyState(SAUCE_KEY_1)) * ((getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) || getInputManager()->getKeyState(SAUCE_KEY_RSHIFT)) ? 5.0f : 1.0f);

		m_generator->m_showNoise = getInputManager()->getKeyState(SAUCE_KEY_SPACE);

		Game::onTick(e);
	}
	
	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		m_generator->draw(e, m_renderTarget.get());

		//Pixmap p = m_renderTarget->getTexture()->getPixmap();;

		context->setShader(m_drawBlocksShader);
		context->drawRectangle(Vector2F(0, 0), getWindow()->getSize());
		context->setShader(0);

		// Draw debug text
		m_spriteBatch->begin();
		m_font->setColor(Color(255));

		stringstream ss;
		ss << "FPS: " << getFPS() << endl;
		ss << "Position: " << m_generator->m_position.toString() << endl;
		ss << "Scale: " << m_generator->m_scale << endl;
		ss << "Seed: " << m_generator->m_seed << endl;
		ss << "Cliffing Factor: " << m_generator->m_cliffingDelta << endl;

		m_font->draw(m_spriteBatch, 5, 5, ss.str());

		m_spriteBatch->end();

		Game::onDraw(e);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	RandomNoiseGPU game;
	return game.run();
}