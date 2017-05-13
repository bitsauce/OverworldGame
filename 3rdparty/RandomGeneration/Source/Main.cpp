#include <Sauce/Sauce.h>

using namespace sauce;

#define BLOCK_EMPTY 0
#define BLOCK_GRASS 1
#define BLOCK_DIRT 2
#define BLOCK_DIRT_BACK 3
#define BLOCK_STONE 4
#define BLOCK_STONE_BACK 5
#define BLOCK_COUNT 6

#define WIDTH 64
#define HEIGHT 64

class RandomGeneration : public Game
{
	Resource<Shader> m_noiseShader, m_drawBlocksShader;
	RenderTarget2D *m_renderTarget;
	Resource<Font> m_font;
	float m_time, m_scale;
	bool m_showNoise;
	Vector2F m_position;
	Random m_rand;
	uint m_seed;
	float m_cliffingDelta;
	SpriteBatch *m_spriteBatch;

public:
	RandomGeneration() :
		Game("RandomGeneration")
	{
	}

	void onStart(GameEvent *e)
	{
		getWindow()->setSize(720, 720);

		m_noiseShader = Resource<Shader>("Generation");
		m_font = Resource<Font>("Font");
		m_renderTarget = new RenderTarget2D(WIDTH, HEIGHT);
		m_spriteBatch = new SpriteBatch();

		m_drawBlocksShader = Resource<Shader>("Draw_Blocks");

		Vector4F blockColors[6];
		blockColors[BLOCK_EMPTY] = Vector4F(0.78, 0.97, 0.96, 1.0);
		blockColors[BLOCK_GRASS] = Vector4F(0.1, 0.8, 0.2, 1.0);
		blockColors[BLOCK_DIRT] = Vector4F(0.3, 0.25, 0.19, 1.0);
		blockColors[BLOCK_DIRT_BACK] = Vector4F(blockColors[BLOCK_DIRT].getRGB() * 0.5, 1.0);
		blockColors[BLOCK_STONE] = Vector4F(0.5, 0.5, 0.5, 1.0);
		blockColors[BLOCK_STONE_BACK] = Vector4F(blockColors[BLOCK_STONE].getRGB() * 0.5, 1.0);

		m_drawBlocksShader->setUniform4f("u_BlockColors", (float*)blockColors);
		m_drawBlocksShader->setSampler2D("u_BlockTexture", m_renderTarget->getTexture());

		getWindow()->setVSync(0);

		m_scale = 1.0f;
		m_showNoise = false;
		m_seed = 0;
		m_cliffingDelta = 70.0;

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		m_time += e->getDelta();

		m_position.x += (getInputManager()->getKeyState(SAUCE_KEY_RIGHT) - getInputManager()->getKeyState(SAUCE_KEY_LEFT)) * 1.0f * ((getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) || getInputManager()->getKeyState(SAUCE_KEY_RSHIFT)) ? 5.0f : 1.0f) * m_scale;
		m_position.y += (getInputManager()->getKeyState(SAUCE_KEY_DOWN) - getInputManager()->getKeyState(SAUCE_KEY_UP)) * 1.0f * ((getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) || getInputManager()->getKeyState(SAUCE_KEY_RSHIFT)) ? 5.0f : 1.0f) * m_scale;

		float prevScale = m_scale;
		if(getInputManager()->getKeyState(SAUCE_KEY_PLUS))
		{
			m_scale /= 1.1;
			m_position -= Vector2F(WIDTH, HEIGHT) * 0.5f * (m_scale - prevScale);
		}
		else if(getInputManager()->getKeyState(SAUCE_KEY_MINUS))
		{
			m_scale *= 1.1;
			m_position -= Vector2F(WIDTH, HEIGHT) * 0.5f * (m_scale - prevScale);
		}

		if(getInputManager()->getKeyState(SAUCE_KEY_0))
		{
			m_seed = m_rand.nextInt(1000);
		}

		m_cliffingDelta += (getInputManager()->getKeyState(SAUCE_KEY_2) - getInputManager()->getKeyState(SAUCE_KEY_1)) * ((getInputManager()->getKeyState(SAUCE_KEY_LSHIFT) || getInputManager()->getKeyState(SAUCE_KEY_RSHIFT)) ? 5.0f : 1.0f);

		if(getInputManager()->getKeyState(SAUCE_KEY_0))
		{
			m_seed = m_rand.nextInt(1000);
		}

		m_showNoise = getInputManager()->getKeyState(SAUCE_KEY_SPACE);

		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		m_noiseShader->setUniform1i("u_ShowNoise", m_showNoise);
		m_noiseShader->setUniform2f("u_Resolution", WIDTH, HEIGHT);
		m_noiseShader->setUniform1f("u_Scale", m_scale);
		m_noiseShader->setUniform2f("u_Position", m_position.x, m_position.y);
		m_noiseShader->setUniform1ui("u_Seed", m_seed);
		m_noiseShader->setUniform1f("u_CliffingDelta", m_cliffingDelta);

		context->setRenderTarget(m_renderTarget);
		context->disable(GraphicsContext::BLEND);
		context->setShader(m_noiseShader);
		context->drawRectangle(0, 0, WIDTH, HEIGHT);
		context->setShader(0);
		context->setRenderTarget(0);

		context->setShader(m_drawBlocksShader);
		context->drawRectangle(Vector2F(0, 0), getWindow()->getSize());
		context->setShader(0);
		context->enable(GraphicsContext::BLEND);

		m_spriteBatch->begin(context);
		m_font->setColor(Color::White);

		stringstream ss;
		ss << "FPS: " << getFPS() << endl;
		ss << "Position: " << m_position.toString() << endl;
		ss << "Scale: " << m_scale << endl;
		ss << "Seed: " << m_seed << endl;
		ss << "Cliffing Factor: " << m_cliffingDelta << endl;

		m_font->draw(m_spriteBatch, 5, 5, ss.str());

		m_spriteBatch->end();

		Game::onDraw(e);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	RandomGeneration game;
	return game.run();
}