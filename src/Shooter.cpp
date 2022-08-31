#include "Shooter.h"
#include "Level.h"
#include <memory>

//================= SHOOTER BASE ==========================
Shooter::Shooter(Level* level_, const Vector2* origin_, const Vector2& centerOffset_) :
	m_origin(origin_),
	m_centerOffset(centerOffset_)
{
	setLevel(level_);
}

void Shooter::setLevel(Level* level_)
{
	m_level = level_;
}

void Shooter::EnterPrecise()
{
	m_isPrecise = true;
}

void Shooter::QuitPrecise()
{
	m_isPrecise = false;
}

void Shooter::draw()
{

}

bool Shooter::getPrecise()
{
	return m_isPrecise;
}


// ========== SHOOTER BASIC ==============
// Controlled by player
// Normal: 2 rocket launchers
Shooter_basic::Shooter_basic(Level* level_, const Vector2* origin_, const Vector2& centerOffset_) :
	Shooter(level_, origin_, centerOffset_),
	m_timePassed(m_period)
{
}

void Shooter_basic::BeginShooting()
{
	//Logger::print("Started shooting\n");
	m_isShooting = IsShooting::LEFT;
}

void Shooter_basic::StopShooting()
{
	//Logger::print("Stopped shooting\n");
	m_isShooting = IsShooting::NOSHOOT;
}

void Shooter_basic::update(const float frameTime_)
{
	m_timePassed += frameTime_;
	if (m_isShooting == IsShooting::NOSHOOT)
		return;

	bool switched = false;
	while (m_timePassed >= m_period)
	{
		switched = true;
		m_timePassed -= m_period;
	}

	if (switched)
	{
		m_level->m_core->soundManager->playSfx(SOUNDS::missle1_launch);
		switch (m_isShooting)
		{
		case (IsShooting::LEFT):
			m_level->create(std::shared_ptr<Object>(new Bullet_missle1(TARGETS::ENEMY, m_level, *m_origin + m_centerOffset - Vector2{ 29, 0 }, { 0, -1 })));
			m_isShooting = IsShooting::RIGHT;
			break;

		case (IsShooting::RIGHT):
			m_level->create(std::shared_ptr<Object>(new Bullet_missle1(TARGETS::ENEMY, m_level, *m_origin + m_centerOffset + Vector2{ 11, 0 }, { 0, -1 })));
			m_isShooting = IsShooting::LEFT;
			break;
		}
	}
}

bool Shooter_basic::getState()
{
	return m_isShooting != IsShooting::NOSHOOT;
}

// ========== SHOOTER LASER ==============
// Controlled by player
// Normal: 1 auto-aim laser
Shooter_laser::Shooter_laser(Level* level_, const Vector2* origin_, const Vector2& centerOffset_) :
	Shooter(level_, origin_, centerOffset_),
	m_timePassed(m_period),
	spr_flash(m_level->m_core->textureManager->getTexture(TEXTURES::laserFlash1))
{
}

void Shooter_laser::BeginShooting()
{
	m_isShooting = true;
	m_target = m_level->FindNearestAngleObject(*m_origin + m_centerOffset);
}

void Shooter_laser::StopShooting()
{
	m_isShooting = false;
	m_target = nullptr;
}

void Shooter_laser::update(const float frameTime_)
{
	m_timePassed += frameTime_;

	if (!m_isShooting || m_target == nullptr)
		return;

	bool isShooted = false;
	while (m_timePassed >= m_period)
	{
		m_timePassed -= m_period;
		isShooted = true;
	}

	if (m_target->m_destroyed)
		m_target = m_level->FindNearestAngleObject(*m_origin + m_centerOffset);

	if (isShooted)
		std::static_pointer_cast<BattleActor>(m_target)->takeDamage(1);
}

bool Shooter_laser::getState()
{
	return m_isShooting;
}

void Shooter_laser::draw()
{
	if (m_isShooting && m_target != nullptr)
	{
		Vector2 src = *m_origin + m_centerOffset;
		Hitbox hBox = m_target->getHitbox();
		m_level->m_core->drawBezierCurve(src, src - Vector2{ 0, 200 }, hBox.getPos() + hBox.getSize() / 2 + Vector2(rand() % 10 - 5, rand() % 10 - 5), 5, { 48, 219, 194, 255 });
		m_level->m_core->renderTexture(spr_flash->getSprite(), src.x - spr_flash->w / 2, src.y - spr_flash->h / 2 - 40);
	}
}

// ========== SHOOTER SPARKLER TARGETED ==============
// Controlled by enemy ship 1
// Normal: 1 auto-aim laser
Shooter_sparkler_targeted::Shooter_sparkler_targeted(Level* level_, const Vector2* origin_, const Vector2& centerOffset_, Player* player_) :
	Shooter(level_, origin_, centerOffset_),
	m_player(player_)
{
}

void Shooter_sparkler_targeted::BeginShooting()
{
	//Logger::print("Started shooting\n");
	m_isShooting = true;
}

void Shooter_sparkler_targeted::StopShooting()
{
	//Logger::print("Stopped shooting\n");
	m_isShooting = false;
}

void Shooter_sparkler_targeted::update(const float frameTime_)
{
	m_timePassed += frameTime_;

	if (!m_isShooting)
		return;

	bool isShooted = false;

	if (m_emitted >= m_sequenceLen)
	{
		while (m_timePassed >= m_sequencePeriod)
		{
			m_timePassed -= m_sequencePeriod;
			isShooted = true;
			m_emitted = 0;
		}
	}
	else
	{
		while (m_timePassed >= m_singlePeriod)
		{
			m_timePassed -= m_singlePeriod;
			isShooted = true;
		}
	}

	if (isShooted)
	{
		//Logger::print("Shooter_sparkler_1 shooted\n");
		Vector2 src = *m_origin + m_centerOffset;
		m_level->create(std::shared_ptr<Object>(new Projectile_sparkle(TARGETS::PLAYER, ANIMATIONS::sparkle5, m_level, src, (m_player->getHitbox().getCenter() - src).normalised())));
		m_emitted++;
	}
		
}

bool Shooter_sparkler_targeted::getState()
{
	return m_isShooting;
}

// ========== SHOOTER SPARKLER CIRCULAR ==============
// Controlled by enemy ship 1
// Normal: circular waves of projectiles
Shooter_sparkler_circular::Shooter_sparkler_circular(Level* level_, const Vector2* origin_, const Vector2& centerOffset_, Player* player_) :
	Shooter(level_, origin_, centerOffset_)
{
}

void Shooter_sparkler_circular::BeginShooting()
{
	m_isShooting = true;
}

void Shooter_sparkler_circular::StopShooting()
{
	m_isShooting = false;
}

void Shooter_sparkler_circular::update(const float frameTime_)
{
	m_timePassed += frameTime_;
	if (m_isShooting == false)
		return;

	bool isShooted = false;

	while (m_timePassed >= m_period)
	{
			m_timePassed -= m_period;
			isShooted = true;
	}

	Vector2 src = *m_origin + m_centerOffset - Vector2(15, 0);

	if (isShooted)
	{
		for (int i = 1; i < density; ++i)
		{
			float angle = 2 * PI * i / density;
			Projectile_sparkle* shootIt = new Projectile_sparkle(TARGETS::PLAYER, ANIMATIONS::sparkle4, m_level, src, Vector2(cos(angle), sin(angle)));
			shootIt->setAbsoluteSpeed(300);
			m_level->create(std::shared_ptr<Object>(shootIt));
		}
	}
}

bool Shooter_sparkler_circular::getState()
{
	return m_isShooting;
}

// ========== SHOOTER SPARKLER SPIRAL ==============
// Controlled by enemy
// Normal: spiral waves of projectiles
Shooter_sparkler_spiral::Shooter_sparkler_spiral(Level* level_, const Vector2* origin_, const Vector2& centerOffset_, Player* player_) :
	Shooter(level_, origin_, centerOffset_)
{
}

void Shooter_sparkler_spiral::BeginShooting()
{
	m_isShooting = true;
}

void Shooter_sparkler_spiral::StopShooting()
{
	m_isShooting = false;
}

void Shooter_sparkler_spiral::update(const float frameTime_)
{
	m_timePassed += frameTime_;
	if (m_isShooting == false)
		return;

	bool isShooted = false;

	while (m_timePassed >= m_period)
	{
		m_timePassed -= m_period;
		isShooted = true;
	}

	const int density = 20;
	Vector2 src = *m_origin + m_centerOffset - Vector2(15, 0);

	if (isShooted)
	{
		float angle = 2 * PI * iter / density;
		Projectile_sparkle *shootIt = new Projectile_sparkle(TARGETS::PLAYER, ANIMATIONS::sparkle4, m_level, src, Vector2(cos(angle), sin(angle)));
		shootIt->setAbsoluteSpeed(250);
		m_level->create(std::shared_ptr<Object>(shootIt));
		++iter;
	}
}

bool Shooter_sparkler_spiral::getState()
{
	return m_isShooting;
}

// ========== SHOOTER HOMING ==============
// Controlled by player
// Normal: 2 homing missles launcher
Shooter_homing::Shooter_homing(Level* level_, const Vector2* origin_, const Vector2& centerOffset_) :
	Shooter(level_, origin_, centerOffset_),
	m_timePassed(m_period)
{
}

void Shooter_homing::BeginShooting()
{
	//Logger::print("Started shooting\n");
	m_isShooting = true;
}

void Shooter_homing::StopShooting()
{
	//Logger::print("Stopped shooting\n");
	m_isShooting = false;
}

void Shooter_homing::update(const float frameTime_)
{
	m_timePassed += frameTime_;
	if (m_isShooting == false)
		return;

	bool isShooted = false;

	if (m_emitted >= m_sequenceLen)
	{
		while (m_timePassed >= m_sequencePeriod)
		{
			m_timePassed -= m_sequencePeriod;
			isShooted = true;
			m_emitted = 0;
		}
	}
	else
	{
		while (m_timePassed >= m_singlePeriod)
		{
			m_timePassed -= m_singlePeriod;
			isShooted = true;
		}
	}

	if (isShooted)
	{
		m_level->m_core->soundManager->playSfx(SOUNDS::missle1_launch);
		m_level->create(std::shared_ptr<Object>(new Bullet_homing(TARGETS::ENEMY, m_level, *m_origin + m_centerOffset - Vector2{ 29, 0 }, { 0, -1 })));
		m_level->create(std::shared_ptr<Object>(new Bullet_homing(TARGETS::ENEMY, m_level, *m_origin + m_centerOffset + Vector2{ 11, 0 }, { 0, -1 })));
		m_emitted++;
	}
}

bool Shooter_homing::getState()
{
	return m_isShooting;
}