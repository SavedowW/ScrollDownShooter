#include "MenuPointer.h"

MenuPointer::MenuPointer(Core* core_, const Vector2& position_) :
	m_core(core_),
	m_position(position_),
	m_target(position_)
{
	m_anim = Animation_t(new Animation(m_core, ANIMATIONS::player_rotate, 0.1));
}

void MenuPointer::moveTo(const Vector2& target_)
{
	m_target = target_;
}

void MenuPointer::update(const float frameTime_)
{
	m_anim->update(frameTime_);

	Vector2 dir = m_target - m_position;
	float dist = dir.getLen();

	if (dist == 0)
		return;

	dir.normalise();
	float spd = maxSpd * frameTime_;
	
	if (dist < smoothDist)
	{
		spd = spd * dist / smoothDist;
	}

	if (spd <= dist)
		m_position += dir * spd;
	else
		m_position = m_target;
}

void MenuPointer::draw()
{
	m_core->renderTexture(m_anim->getSprite(), m_position.x, m_position.y, 80, 80);
}
