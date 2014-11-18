#include "bone.h"

spBoneWrapper::spBoneWrapper(spBone *bone) :
	m_self(bone)
{
}

void spBoneWrapper::setPosition(const Vector2 &position)
{
	m_self->x = position.x;
	m_self->y = position.y;
}

void spBoneWrapper::setRotation(const float rotation)
{
	m_self->rotation = rotation;
}

void spBoneWrapper::setScale(const Vector2 &scale)
{
	m_self->scaleX = scale.x;
	m_self->scaleY = scale.y;
}

Vector2 spBoneWrapper::getWorldPosition() const
{
	return Vector2(m_self->worldX, m_self->worldY);
}

float spBoneWrapper::getWorldRotation() const
{
	return m_self->worldRotation;
}

Vector2 spBoneWrapper::getWorldScale() const
{
	return Vector2(m_self->worldScaleX, m_self->worldScaleY);
}