#include "background.h"
#include "timeofday.h"

#include "constants.h"

#include "game/world.h"

XColor mixColors(XColor c1, XColor c2, const float a)
{
	return c1 * a + c2 * (1.0f - a);
}

Background::Background(TimeOfDay *timeOfDay) :
	GameObject(DRAW_ORDER_BACKGROUND),
	m_timeOfDay(timeOfDay),
	m_topColor(255, 255, 255, 255),
	m_bottomColor(90, 170, 255, 255),
	m_wind(0.04f),
	m_cloudTime(0.0f)
{
	m_sun = new XSprite(XTextureRegion(xd::ResourceManager::get<XTexture>(":/sprites/sky/sun.png")));
	m_moon = new XSprite(XTextureRegion(xd::ResourceManager::get<XTexture>(":/sprites/sky/moon.png")));
		
	m_sun->setOrigin(m_sun->getCenter());
	m_moon->setOrigin(m_moon->getCenter());
}

void Background::update()
{
	// Get hour and mintue
	int hour = m_timeOfDay->getHour();
	int minute = m_timeOfDay->getMinute();
	float time = m_timeOfDay->getTime();
		
	// Change background depending on time
	if(m_timeOfDay->isDay())
	{
		// Apply sunrise from 6:00 to 9:00
		if(hour >= 6 && hour < 9)
		{
			// Percentage of sunrise
			float minscale = 1.0f - (540-time)/180.0f; // Aka. (9*60-time)/(6*60-9*60)
			m_topColor = mixColors(XColor(255, 255, 255), XColor(0, 0, 0), minscale);
			m_bottomColor = mixColors(XColor(90, 170, 255), XColor(10, 60, 110), minscale);
		}
		else
		{
			// Set day gradient
			m_topColor = XColor(255, 255, 255);
			m_bottomColor = XColor(90, 170, 255);
		}
			
		// Place sun
		float ang = (1140-time)/720.0f;
		Vector2 windowSize = XWindow::getSize();
		Vector2 sunSize = m_sun->getSize();
		m_sun->setPosition(windowSize.x/2.0f - sunSize.x/2.0f + cos(PI*ang) * (windowSize.x/2.0f + sunSize.x/4.0f), windowSize.y/2.0f - sin(PI*ang) * (windowSize.y/2.0f + 64));
		m_sun->setRotation(180*(1.0f-ang));
	}
	else
	{
		// Apply sunset from 18:00 to 21:00
		if(hour >= 18 && hour < 21)
		{
			// Percentage of sunset
			float minscale = 1.0f - (1260-time)/180.0f; // Aka. (21*60-time)/(18*60-21*60)
			m_topColor = mixColors(XColor(0, 0, 0, 255), XColor(255, 255, 255), minscale);
			m_bottomColor = mixColors(XColor(10, 60, 110, 255), XColor(90, 170, 255), minscale);
		}else
		{
			// Set night gradient
			m_topColor = XColor(0, 0, 0);
			m_bottomColor = XColor(10, 60, 110);
		}
				
		// Place moon
		float ang = (1860 - (time >= 1140 ? time : time + 1440))/720.0f;
		Vector2 windowSize = XWindow::getSize();
		Vector2 moonSize = m_moon->getSize();
		m_moon->setPosition(windowSize.x/2.0f - moonSize.x/2.0f + cos(PI*ang) * (windowSize.x/2.0f + moonSize.x/2.0f), windowSize.y/2.0f - sin(PI*ang) * windowSize.y/2.0f);
		m_moon->setRotation(180*(1.0f-ang));
	}
		
	// Apply wind
	m_cloudTime += m_wind * XGraphics::getTimeStep();
}

void Background::draw(XBatch *batch)
{
	// Draw sky gradient
	vector<XVertex> vertices(4);
	vertices[0].set4f(VERTEX_POSITION, 0.0f, 0.0f);
	vertices[0].set4ub(VERTEX_COLOR, m_topColor.r, m_topColor.g, m_topColor.b, m_topColor.a);
	vertices[1].set4f(VERTEX_POSITION, XWindow::getSize().x, 0.0f);
	vertices[1].set4ub(VERTEX_COLOR, m_topColor.r, m_topColor.g, m_topColor.b, m_topColor.a);
	vertices[2].set4f(VERTEX_POSITION, XWindow::getSize().x, XWindow::getSize().y);
	vertices[2].set4ub(VERTEX_COLOR, m_bottomColor.r, m_bottomColor.g, m_bottomColor.b, m_bottomColor.a);
	vertices[3].set4f(VERTEX_POSITION, 0, XWindow::getSize().y);
	vertices[3].set4ub(VERTEX_COLOR, m_bottomColor.r, m_bottomColor.g, m_bottomColor.b, m_bottomColor.a);
		
	batch->addVertices(vertices.data(), 4, QUAD_INDICES, 6);
	
	int hour = m_timeOfDay->getHour();
	if(hour >= 6 && hour < 18)
	{
		m_sun->draw(batch);
	}
	else
	{
		m_moon->draw(batch);
	}
		
	// Draw background elements
	/*hill1.setSize(XWindow::getSize().x, XWindow::getSize().y);
	hill1.setPosition(XMath::mod(-Camera.position.x * 0.25f, Window.getSize().x*2) - Window.getSize().x, Window.getSize().y-hill1.getHeight());
	hill1.draw(@background);
	hill1.setPosition(Math.mod(-Camera.position.x * 0.25f + Window.getSize().x, Window.getSize().x*2) - Window.getSize().x, Window.getSize().y-hill1.getHeight());
	hill1.draw(@background);*/
}