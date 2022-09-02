#include "Projectiles.h"
#include "Level.h"
#include <memory>

template<class ParticleType>
Projectile<ParticleType>::Projectile(int damage_, TARGETS target_, Level* level_, const Hitbox& hBox_, SOUNDS deathSound_, const Vector2& position_, const Vector2& velocity_) :
	Object(level_, hBox_, OBJ_TAGS::BULLET, position_, velocity_),
	m_deathSound(deathSound_),
	m_damage(damage_),
	m_target(target_)
{
	setVelocity(m_velocity);
}

template<class ParticleType>
void Projectile<ParticleType>::setVelocity(const Vector2& velocity_)
{
	m_velocity = velocity_;
	m_angle = atan2(-m_velocity.x, m_velocity.y) * 180 / PI + 180;
}

template<class ParticleType>
int Projectile<ParticleType>::getDamage()
{
	return m_damage;
}

template<class ParticleType>
void Projectile<ParticleType>::update(const float frameTime_)
{
	moveSelf(frameTime_);

	spr_idle->update(frameTime_);
}

template<class ParticleType>
void Projectile<ParticleType>::draw()
{
	m_core->renderTexture(spr_idle.get()->getSprite(), m_position.x, m_position.y, m_angle, NULL, SDL_FLIP_NONE);

#ifdef SHOWHITBOXES
	drawHbox();
#endif
}

template<class ParticleType>
void Projectile<ParticleType>::moveSelf(const float frameTime_)
{
	m_position += m_velocity * frameTime_;
	Hitbox hbox = getHitbox();
	Vector2 lvlSize = m_level->getSize();
	if (hbox.x > lvlSize.x ||
		hbox.y > lvlSize.y ||
		hbox.x + hbox.w < 0 ||
		hbox.y + hbox.h < 0)
	{
		m_outfield = true;
		m_level->destroy(this);
	}
}

//Particle with angle
template<>
void Projectile<Particle_hit1>::destroySelf()
{
	m_level->destroy(this);
	if (!m_outfield)
	{
		Object* ptr = reinterpret_cast<Object*>(new Particle_hit1(m_level, m_angle, m_position));
		m_level->create(std::shared_ptr<Object>(ptr));
		m_core->soundManager->playSfx(m_deathSound);
	}
}

//No particle at all
template<>
void Projectile<Particle>::destroySelf()
{
	m_level->destroy(this);
	if (!m_outfield)
		m_core->soundManager->playSfx(m_deathSound);
}

//Non-angle particles
template<class ParticleType>
void Projectile<ParticleType>::destroySelf()
{
	m_level->destroy(this);
	if (!m_outfield)
	{
		Object* ptr = reinterpret_cast<Object*>(new ParticleType(m_level, m_position));
		m_level->create(std::shared_ptr<Object>(ptr));
		m_core->soundManager->playSfx(m_deathSound);
	}
}

// ========== MISSLE 1 =============
Bullet_missle1::Bullet_missle1(TARGETS target_, Level* level_, const Vector2& position_, const Vector2& direction_) :
	Projectile(2, target_, level_, Hitbox(3, 10, 12, 12), SOUNDS::missle1_destroyed, position_, direction_ * 3200)
{
	spr_idle = m_core->textureManager->getTexture(TEXTURES::missle1);
}

// ========== SPARKLE 5 =============
Projectile_sparkle::Projectile_sparkle(TARGETS target_, ANIMATIONS anim_, Level* level_, const Vector2& position_, const Vector2& direction_) :
	Projectile(1, target_, level_, Hitbox(11, 10, 27, 22), SOUNDS::missle1_destroyed, position_, direction_ * 500)
{
	spr_idle = Animation_t( new Animation(m_core, anim_, 0.03, LOOPMETHOD::SWITCH_DIR_LOOP));
}

void Projectile_sparkle::draw()
{
	m_core->renderTexture(spr_idle.get()->getSprite(), m_position.x, m_position.y, 50, 43);

#ifdef SHOWHITBOXES
	drawHbox();
#endif
}

void Projectile_sparkle::setAbsoluteSpeed(const float speed_)
{
	m_velocity = m_velocity.normalised() * speed_;
}

// ========== HOMING MISSLE =============
Bullet_homing::Bullet_homing(TARGETS target_, Level* level_, const Vector2& position_, const Vector2& direction_) :
	Projectile(1, target_, level_, Hitbox(3, 10, 12, 12), SOUNDS::homingMissle_death, position_, direction_ * 100)
{
	spr_idle = Animation_t(new Animation(m_core, ANIMATIONS::torp_autoaim, 0.03));
}

void Bullet_homing::update(const float frameTime_)
{
	if (!m_aimingAt || m_aimingAt->m_destroyed)
	{
		m_aimingAt = m_level->FindNearestAngleObject(m_position);
	}

	if (m_aimingAt)
	{
		//Current position of target and distance to it
		Vector2 targetPos = m_aimingAt->getHitbox().getCenter();
		float dist = (targetPos - m_position).getLen();

		//Trying to predict position of a target at the moment of colliding,
		//but use time to get to current target position so its not 100%
		targetPos += m_aimingAt->getVelocity() * sqrt(2 * dist / m_accel);
		setVelocity(m_velocity + (targetPos - m_position).normalised() * m_accel * frameTime_);
	}

	else
	{
		setVelocity(m_velocity + m_velocity.normalised() * m_accel * frameTime_);
	}

	moveSelf(frameTime_);

	spr_idle->update(frameTime_);
}
