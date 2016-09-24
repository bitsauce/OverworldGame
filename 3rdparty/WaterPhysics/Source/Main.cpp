#include <Sauce/Sauce.h>

using namespace sauce;

class WaterPhysicsGame : public Game
{
	Resource<Shader> m_waterTickShader;

	int m_currentWaterTextureIndex;
	shared_ptr<Texture2D> m_waterTextureBuffers[2];

	GraphicsContext *m_graphicsContext;

public:
	WaterPhysicsGame() :
		Game("WaterPhysics")
	{
	}
	
	void onStart(GameEvent *e)
	{
		m_graphicsContext = getWindow()->getGraphicsContext();

		m_waterTickShader = Resource<Shader>("Shader/Water_Tick");

		for(int i = 0; i < 2; i++)
		{
			m_waterTextureBuffers[i] = shared_ptr<Texture2D>(new Texture2D(80, 45));
			m_waterTextureBuffers[i]->setWrapping(Texture2D::REPEAT);
		}

		uchar pixel[4] = { 0, 0, 255, 255 };
		m_waterTextureBuffers[0]->updatePixmap(40, 0, Pixmap(1, 1, pixel));

		m_currentWaterTextureIndex = 0;
		m_graphicsContext->disable(GraphicsContext::BLEND);

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		RenderTarget2D waterRenderTarget(m_waterTextureBuffers[1 - m_currentWaterTextureIndex]);
		m_graphicsContext->setRenderTarget(&waterRenderTarget);

		m_waterTickShader->setSampler2D("u_Texture", m_waterTextureBuffers[m_currentWaterTextureIndex]);

		m_graphicsContext->setShader(m_waterTickShader);
		m_graphicsContext->drawRectangle(0, 0, 80, 45);
		m_graphicsContext->setShader(0);

		m_graphicsContext->setRenderTarget(0);

		m_currentWaterTextureIndex = 1 - m_currentWaterTextureIndex;

		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		m_graphicsContext->setTexture(m_waterTextureBuffers[m_currentWaterTextureIndex]);
		m_graphicsContext->drawRectangle(0, 0, 80 * 16, 45 * 16);
		m_graphicsContext->setTexture(0);

		Game::onDraw(e);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	WaterPhysicsGame game;
	return game.run();
}