#include "Particle.h"
#include "Level.h"

Particle::Particle(Level* level_, float angle_, const Vector2& position_, const SDL_Point& center_) :
	Object(level_, Hitbox(0, 0, 0, 0), OBJ_TAGS::PARTICLE, position_ - Vector2(center_.x, center_.y)),
	m_center(center_),
	m_angle(angle_)
{
	m_hitbox.x = m_center.x - 1;
	m_hitbox.y = m_center.y - 1;
	m_hitbox.w = 2;
	m_hitbox.h = 2;
}

void Particle::update(const float frameTime_)
{
	m_anim->update(frameTime_);
	if (m_anim->isFinished())
		m_level->destroy(this);
}

void Particle::draw()
{
	m_core->renderTexture(m_anim->getSprite(), m_position.x, m_position.y, m_angle, &m_center, SDL_FLIP_NONE);
}

Particle::~Particle()
{
}

//============= HIT 1 ================
Particle_hit1::Particle_hit1(Level* level_, float angle_, const Vector2& position_) :
	Particle(level_, angle_, position_, {82, 7})
{
	m_anim = Animation_t(new Animation(m_core, ANIMATIONS::hit1, 0.016, LOOPMETHOD::NOLOOP));
}


//============= EXPLOSION 1 ================
Particle_expl1::Particle_expl1(Level* level_, const Vector2& position_) :
	Particle(level_, 0, position_, { 120, 115 })
{
	m_anim = Animation_t(new Animation(m_core, ANIMATIONS::explosion1, 0.03, LOOPMETHOD::NOLOOP));
}


//============= EXPLOSION 2 ================
Particle_expl2::Particle_expl2(Level* level_, const Vector2& position_) :
	Particle(level_, 0, position_, { 32, 32 })
{
	m_anim = Animation_t(new Animation(m_core, ANIMATIONS::explosion2, 0.03, LOOPMETHOD::NOLOOP));
}

//============= EXPLOSION 3 ================
Particle_expl3::Particle_expl3(Level* level_, const Vector2& position_) :
	Particle(level_, 0, position_, { 170, 120 })
{
	m_anim = Animation_t(new Animation(m_core, ANIMATIONS::explosion3, 0.07, LOOPMETHOD::NOLOOP));
}

Particle_expl4::Particle_expl4(Level* level_, const Vector2& position_) :
	Particle(level_, 0, position_, { 32, 32 })
{
	m_anim = Animation_t(new Animation(m_core, ANIMATIONS::explosion4, 0.01, LOOPMETHOD::NOLOOP));
}
