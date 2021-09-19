#include "BattleActor.h"
#include "Level.h"

BattleActor::BattleActor(int maxHP_, int maxSpd_, Level* level_, const Hitbox& hBox_, const Vector2& position_, const Vector2& velocity_) :
    Object(level_, hBox_, OBJ_TAGS::OBJECT, position_, velocity_),
    m_maxHP(maxHP_),
    m_HP(maxHP_),
    m_maxSpd(maxSpd_)
{
}

void BattleActor::takeDamage(int damage_)
{
    if (damage_ < 0)
        throw std::runtime_error("Taken damage is lower than 0");
    m_HP -= damage_;
    reactOnDamage();
    if (m_HP <= 0)
    {
        Logger::print("Calling die()\n");
        die();
    }
}

void BattleActor::die()
{
}

void BattleActor::reactOnDamage()
{
}

const Vector2 &BattleActor::getShootDir()
{
    return Vector2(0, 1);
}

BattleActor::~BattleActor()
{
}
