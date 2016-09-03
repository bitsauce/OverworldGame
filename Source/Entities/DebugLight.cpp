#include "DebugLight.h"

#include "Game/Game.h"
#include "World/Controllers/Lighting.h"

list<DebugPointlight*> DebugPointlight::s_pointlights;

DebugPointlight::DebugPointlight(World *world) :
	Entity(world, ENTITY_DEBUG_POINTLIGHT),
	m_pointlight(new Pointlight(world))
{
	m_pointlight->setMobility(LightSource::DYNAMIC);
	m_pointlight->setRadius(0.0f);
	m_pointlight->setColor(Color(255));

	s_pointlights.push_back(this);
}

DebugPointlight::~DebugPointlight()
{
	delete m_pointlight;
	s_pointlights.remove(this);
}

void DebugPointlight::onTick(TickEvent *e)
{
	m_pointlight->setPosition(getPosition() / BLOCK_PXF);
}

void DebugPointlight::onSaveData(FileWriter &f)
{
	f << getX() << endl;
	f << getY() << endl;
	f << m_pointlight->getRadius() << endl;
	Color c = m_pointlight->getColor();
	f << c.x << endl;
	f << c.y << endl;
	f << c.z << endl;
	f << c.w << endl;
}

void DebugPointlight::onLoadData(FileReader &f)
{
	float x, y, radius;
	f >> x;
	f >> y;
	setPosition(x, y);

	f >> radius;
	m_pointlight->setRadius(radius);
	
	int r, g, b, a;
	f >> r;
	f >> g;
	f >> b;
	f >> a;
	m_pointlight->setColor(Color(r, g, b, a));
}

Pointlight *DebugPointlight::getPointlight() const
{
	return m_pointlight;
}
