#include "Object.h"
#include "Level.h"

Object::Object(Level* level_, const Hitbox& hitbox_, OBJ_TAGS tag_, const Vector2& position_, const Vector2& velocity_) :
	m_position(position_),
	m_velocity(velocity_),
	m_hitbox(hitbox_),
	m_tag(tag_)
{
	setLevel(level_);
}

Hitbox Object::getHitbox() const
{
	return m_hitbox + m_position;
}

const Vector2 &Object::getPosition() const
{
	return m_position;
}

void Object::setVelocity(const Vector2& velocity_)
{
	m_velocity = velocity_;
}

const Vector2 &Object::getVelocity()
{
	return m_velocity;
}

void Object::update(const float frameTime_)
{
	m_position += m_velocity * frameTime_;
}

void Object::draw()
{
	drawHbox();
}

void Object::drawHbox(const SDL_Color& color_)
{
	Hitbox trueHbox = getHitbox();
	m_core->drawFilledRect(trueHbox.getPos(), trueHbox.getSize(), color_);
}

void Object::setLevel(Level* level_)
{
	m_level = level_;
}

Object::~Object()
{
}
