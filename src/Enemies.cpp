#include "Enemies.h"
#include "Level.h"

template<class ParticleType>
Enemy<ParticleType>::Enemy(Level* level_, Player* player_, int maxHP_, int maxSpd_, const Hitbox& hBox_, SOUNDS deathSfx_, const Vector2& position_) :
	BattleActor(maxHP_, maxSpd_, level_, hBox_, position_, Vector2{ 0, 0 }),
	m_player(player_),
	m_damageTimer(m_damageLightPeriod),
	m_deathSfx(deathSfx_)
{
}

template<class ParticleType>
void Enemy<ParticleType>::update(const float frameTime_)
{
	updateVelocity();

	moveSelf(frameTime_);

	if (m_damageTimer < m_damageLightPeriod)
		m_damageTimer += frameTime_;

	auto step1 = m_level->checkBulletCollision(this, TARGETS::ENEMY);
	auto collidedWith = static_pointer_cast<Projectile<class ParticleType>>(step1);
	if (collidedWith)
	{
		takeDamage(collidedWith->getDamage());
		collidedWith->destroySelf();
	}

	spr_anim->update(frameTime_);

	localUpdateLogic(frameTime_);
}

template<class ParticleType>
void Enemy<ParticleType>::updateVelocity()
{
}

template<class ParticleType>
void Enemy<ParticleType>::localUpdateLogic(const float frameTime_)
{
	if (getHitbox().isCollideWith(m_player->getHitbox()))
	{
		m_player->takeDamage(1);
	}
}

template<class ParticleType>
void Enemy<ParticleType>::draw()
{
	bool isDamaged = m_damageTimer < m_damageLightPeriod;

	SDL_Texture* currentTex = spr_anim->getSprite();

	if (isDamaged)
		SDL_SetTextureColorMod(currentTex, 255, 100, 100);
	m_core->renderTexture(spr_anim->getSprite(), m_position.x, m_position.y, m_angle, NULL, SDL_FLIP_NONE);
	if (isDamaged)
		SDL_SetTextureColorMod(currentTex, 255, 255, 255);

	//Draw HP bar
	int w = m_hitbox.w;
	m_core->drawOutRect(m_position - Vector2(0, 8), Vector2(w, 6), SDL_Color(255, 0, 0, 150));
	m_core->drawFilledRect(m_position - Vector2(-2, 6), Vector2((w - 3) * float(m_HP) / m_maxHP, 3), SDL_Color(0, 255, 0, 200));


#ifdef SHOWHITBOXES
	drawHbox();
#endif
}

template<class ParticleType>
void Enemy<ParticleType>::reactOnDamage()
{
	m_damageTimer = 0;
}

template<class ParticleType>
void Enemy<ParticleType>::die()
{
	m_level->destroy(this);
	if (!m_offScreen)
	{
		std::shared_ptr<ParticleType> ptr = std::make_shared<ParticleType>(m_level, m_position + m_hitbox.getSize() / 2);
		m_level->create(std::reinterpret_pointer_cast<Particle>(ptr));
		m_core->soundManager->playSfx(m_deathSfx);
	}
}

template<class ParticleType>
void Enemy<ParticleType>::moveSelf(const float frameTime_)
{
	m_position += m_velocity * frameTime_;
	Hitbox hbox = getHitbox();
	Vector2 lvlSize = m_level->getSize();
	if (hbox.x + hbox.w > 0 &&
		hbox.y + hbox.h > 0 &&
		hbox.x < lvlSize.x &&
		hbox.y < lvlSize.y)
	{
		//If in screen
		m_touchedScreen = true;
	}
	else if (m_touchedScreen)
	{
		//If left the screen
		m_offScreen = true;
		die();
	}
}

// ========== SWARMER ==================
// Swarmer slowly moves towards the player and explodes when nearby
Swarmer::Swarmer(Level* level_, Player* player_, const Vector2& position_) :
	Enemy(level_, player_, 10, 200, Hitbox(0, 0, 80, 80), SOUNDS::swarmer_death, position_)
{
	spr_anim = std::shared_ptr<Animation>(new Animation(m_core, ANIMATIONS::swarmer, 0.1));
}

void Swarmer::updateVelocity()
{
	//Update velocity
	m_velocity = (m_player->getPosition() - m_position);
	m_velocity.normalise();
	m_velocity *= m_maxSpd;
}

void Swarmer::localUpdateLogic(const float frameTime)
{
	if (getHitbox().isCollideWith(m_player->getHitbox()))
	{
		m_player->takeDamage(1);
		die();
	}
}

void Swarmer::draw()
{
	bool isDamaged = m_damageTimer < m_damageLightPeriod;

	SDL_Texture* currentTex = spr_anim->getSprite();

	if (isDamaged)
		SDL_SetTextureColorMod(currentTex, 255, 100, 100);
	m_core->renderTexture(spr_anim->getSprite(), m_position.x, m_position.y);
	if (isDamaged)
		SDL_SetTextureColorMod(currentTex, 255, 255, 255);

	//Draw HP bar
	int w = m_hitbox.w;
	m_core->drawOutRect(m_position - Vector2(0, 8), Vector2(w, 6), SDL_Color(255, 0, 0, 150));
	m_core->drawFilledRect(m_position - Vector2(-2, 6), Vector2((w - 3) * float(m_HP) / m_maxHP, 3), SDL_Color(0, 255, 0, 200));


#ifdef SHOWHITBOXES
	drawHbox();
#endif
}

// ========== Enemy Ship 1 ==================
// Moves is static direction and periodically shoot the player
template<class ShooterType>
EnemyShip1<ShooterType>::EnemyShip1(Level* level_, Player* player_, const Vector2& position_, const Vector2& velocity_) :
	Enemy(level_, player_, 3, 175, Hitbox(10, 5, 75, 60), SOUNDS::swarmer_death, position_)
{
	m_velocity = velocity_;
	m_velocity.normalise();
	m_velocity *= m_maxSpd;
	m_angle = atan2(-m_velocity.x, m_velocity.y) * 180 / PI;

	spr_anim = std::shared_ptr<Animation>(new Animation(m_core, ANIMATIONS::enemyShip1, 0.1, LOOPMETHOD::SWITCH_DIR_LOOP));
	m_shooter = std::shared_ptr<Shooter>(new ShooterType(m_level, &m_position, m_hitbox.getCenter(), m_player));
	m_shooter->BeginShooting();
}

template<class ShooterType>
void EnemyShip1<ShooterType>::localUpdateLogic(const float frameTime_)
{
	m_shooter->update(frameTime_);

	if (getHitbox().isCollideWith(m_player->getHitbox()))
	{
		m_player->takeDamage(1);
	}
}