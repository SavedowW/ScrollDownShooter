#ifndef BATTLE_ACTOR_H_
#define BATTLE_ACTOR_H_
#include "Object.h"
#include "Projectiles.h"

//Base class for player and enemies
class BattleActor : public Object
{
public:
	BattleActor(int maxHP_, int maxSpd_, Level* level_, const Hitbox& hBox_, const Vector2& position_ = Vector2{ 0, 0 }, const Vector2& velocity_ = Vector2{ 0, 0 });
	virtual void takeDamage(int damage_);
	virtual void die();
	virtual void reactOnDamage();

	const virtual Vector2 &getShootDir();

	virtual ~BattleActor() = default;

protected:
	int m_HP;
	int m_maxHP;
	int m_maxSpd;
};

#endif