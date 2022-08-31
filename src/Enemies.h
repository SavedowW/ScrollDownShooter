#ifndef ENEMIES_H_
#define ENEMIES_H_
#include "BattleActor.h"
#include "Player.h"

template<class ParticleType>
class Enemy : public BattleActor
{
public:
	Enemy(Level* level_, Player* player_, int maxHP_, int maxSpd_, const Hitbox& hBox_, SOUNDS deathSfx_, const Vector2& position_ = Vector2{ 0, 0 });
	virtual void update(const float frameTime_) override;
	virtual void updateVelocity();
	virtual void localUpdateLogic(const float frameTime_);
	virtual void draw() override;
	virtual void reactOnDamage() override;
	virtual void die() override;

	virtual ~Enemy() = default;

protected:
	bool m_touchedScreen = false;
	bool m_offScreen = false;
	float m_angle = 0;
	void moveSelf(const float frameTime_);
	float m_damageTimer;
	const float m_damageLightPeriod = 0.030f;
	Player* m_player;
	Sprite_t spr_anim;
	SOUNDS m_deathSfx;

};

class Swarmer : public Enemy<Particle_expl1>
{
public:
	Swarmer(Level* level_, Player* player_, const Vector2& position_ = Vector2{ 0, 0 });
	void updateVelocity() final;
	void localUpdateLogic(const float frameTime) final;
	void draw() final;

};

template<class ShooterType>
class EnemyShip1 : public Enemy<Particle_expl1>
{
public:
	EnemyShip1(Level* level_, Player* player_, const Vector2& position_ = Vector2{ 0, 0 }, const Vector2& velocity_ = Vector2{ 0, 0 });
	void localUpdateLogic(const float frameTime_) final;

private:
	std::shared_ptr<Shooter> m_shooter;

};

template class Enemy<Particle_expl1>;

template class EnemyShip1<Shooter_sparkler_targeted>;
template class EnemyShip1<Shooter_sparkler_circular>;
template class EnemyShip1<Shooter_sparkler_spiral>;

#endif