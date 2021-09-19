#ifndef PLAYER_H_
#define PLAYER_H_
#include "BattleActor.h"
#include "Projectiles.h"
#include "Shooter.h"

class Shooter;

class Player : public BattleActor, public InputReactor
{
public:
	Player(Level* level_, const Vector2& position_ = Vector2{ 0, 0 });

	//Object funcs override
	void update(const float frameTime_) final;
	void draw() final;
	void setLevel(Level* level_) final;

	//Input override
	void input_axisupd(const float scale_) final;
	void input_ok(const float scale_) final;
	void input_cancel(const float scale_) final;
	void input_shift(const float scale_) final;

	//BattleActor override
	const Vector2 &getShootDir() override;
	void takeDamage(int damage_) final;

	void setPosition(const Vector2 &pos_);

	~Player();

private:
	Vector2 m_offsetToCenter;
	float m_speedQuaf = 1; //Used for precise movement
	std::shared_ptr<Texture> spr_idle;
	std::shared_ptr<Animation> spr_moveRight;
	std::shared_ptr<Animation> spr_moveLeft;
	enum class CurrentAnimation
	{
		RIGHT,
		LEFT,
		IDLE
	} m_currentAnimation;

	std::vector<Shooter*> m_shooters;
	std::vector<Shooter*>::iterator m_shooter;

	Animation_t spr_heath;

	float m_invincibilityTime = false;
};

#endif