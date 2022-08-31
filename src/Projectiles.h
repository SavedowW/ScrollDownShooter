#ifndef PROJECTILES_H_
#define PROJECTILES_H_
#include "Object.h"
#include "Particle.h"

enum class TARGETS {
	PLAYER,
	ENEMY
};

//Override draw to use
template<class ParticleType>
class Projectile : public Object
{
public:
	Projectile(int damage_, TARGETS target_, Level* level_, const Hitbox& hBox_, SOUNDS deathSound_, const Vector2& position_ = Vector2{ 0, 0 }, const Vector2& velocity_ = Vector2{ 0, 0 });
	virtual void setVelocity(const Vector2 &velocity) override;
	virtual int getDamage();
	virtual void update(const float frameTime_) override;
	virtual void destroySelf();
	virtual void draw() override;
	virtual ~Projectile() = default;
	TARGETS m_target;

protected:
	Sprite_t spr_idle;
	virtual void moveSelf(const float frameTime_);
	SOUNDS m_deathSound;
	bool m_outfield = false;
	int m_damage;
	float m_angle;

};

//========== Missle 1 =============
class Bullet_missle1 : public Projectile<Particle_hit1>
{
public:
	Bullet_missle1(TARGETS target_, Level* level_, const Vector2& position_, const Vector2& direction_);

protected:

};

//========== Sparkle =============
class Projectile_sparkle : public Projectile<Particle>
{
public:
	Projectile_sparkle(TARGETS target_, ANIMATIONS anim_, Level * level_, const Vector2 & position_, const Vector2 & direction_);
	void setAbsoluteSpeed(const float speed_);
	void draw() final;

protected:

};

//========== Homing missle =============
class Bullet_homing : public Projectile<Particle_expl4>
{
public:
	Bullet_homing(TARGETS target_, Level* level_, const Vector2& position_, const Vector2& direction_);
	void update(const float frameTime_) final;

protected:
	const int m_accel = 3000;
	std::shared_ptr<Object> m_aimingAt = nullptr;

};

//Template predefinitions
 template class Projectile<Particle_hit1>;
 template class Projectile<Particle>;
 template class Projectile<Particle_expl4>;

#endif