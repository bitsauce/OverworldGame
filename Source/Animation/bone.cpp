#include "bone.h"

Bone::Bone(spBone *bone) :
	m_self(bone)
{
}

void Bone::setPosition(const Vector2F &position)
{
	m_self->x = position.x;
	m_self->y = position.y;
}

void Bone::setRotation(const float rotation)
{
	m_self->rotation = rotation;
}

void Bone::setScale(const Vector2F &scale)
{
	m_self->scaleX = scale.x;
	m_self->scaleY = scale.y;
}

Vector2F Bone::getWorldPosition() const
{
	return Vector2F(m_self->worldX, m_self->worldY);
}

float Bone::getWorldRotation() const
{
	return m_self->worldRotation;
}

Vector2F Bone::getWorldScale() const
{
	return Vector2F(m_self->worldScaleX, m_self->worldScaleY);
}