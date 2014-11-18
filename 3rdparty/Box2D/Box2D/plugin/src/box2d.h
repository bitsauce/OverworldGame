#ifndef B2_BOX2D_H
#define B2_BOX2D_H

#include <x2d/x2d.h>
#include <Box2D/Box2D.h>
#include "body.h"

class Box2D
{
public:
	Box2D();
	~Box2D();

	void step(float timeStep);
	void draw(XBatch *batch);
	void setDrawFlags(int flags);
	void setScale(float scale);
	float getScale() const;
	void setGravity(const Vector2 &gravity);
	Vector2 getGravity() const;
	b2World *getWorld() const;
	bool isLocked() const;

	XBatch *getDrawBatch() const;

	void createJoint(b2Joint **joint, const b2JointDef &jointDef);

	void destroyBody(b2Body **body);
	void destroyFixture(b2Body *body, b2Fixture **fixture);

private:
	float m_scale;
	b2World *m_world;
	b2Draw *m_debugDraw;
	b2ContactListener *m_contactListener;

	set<pair<b2Joint**, b2JointDef*>> m_jointsToCreate;

	set<b2Body**> m_bodiesToDestroy;
	set<pair<b2Body*, b2Fixture**>> m_fixturesToDestroy;

	XBatch *m_drawBatch;
};

extern Box2D *b2d;

extern b2Vec2 toB2Vec(const Vector2 &vec);
extern Vector2 toXDVec(const b2Vec2 &vec);

#endif // B2_BOX2D_H