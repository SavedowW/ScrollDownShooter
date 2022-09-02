#include "Player.h"
#include "Level.h"

Player::Player(Level* level_, const Vector2& position_) :
	BattleActor(5, 1000, level_, Hitbox(0, 0, 32, 32), m_position, Vector2(0, 0)),
	InputReactor(&m_core->input)
{
	Logger::print("Hello world!\n");

	spr_idle = m_core->textureManager->getTexture(TEXTURES::player_idle);
	spr_moveLeft = Animation_t(new Animation(m_core, ANIMATIONS::player_moveLeft, 0.1, LOOPMETHOD::NOLOOP, 2, -1));
	spr_moveRight = Animation_t(new Animation(m_core, ANIMATIONS::player_moveRight, 0.1, LOOPMETHOD::NOLOOP));
	m_currentAnimation = CurrentAnimation::IDLE;

	spr_heath = Animation_t(new Animation(m_core, ANIMATIONS::player_rotate, 0.1));

	int hw = 32;
	int hh = 32;
	m_hitbox.x = (spr_idle->w - hw) / 2;
	m_hitbox.y = (spr_idle->h - hh) / 2;
	m_hitbox.w = hw;
	m_hitbox.h = hh;

	m_core->input.subscribe(EVENTS::AXISUPD, dynamic_cast<InputReactor*>(this));
	m_core->input.subscribe(EVENTS::OK, dynamic_cast<InputReactor*>(this));
	m_core->input.subscribe(EVENTS::CANCEL, dynamic_cast<InputReactor*>(this));
	m_core->input.subscribe(EVENTS::SHIFT, dynamic_cast<InputReactor*>(this));

	m_shooters.push_back(std::make_unique<Shooter_basic>(m_level, &m_position, m_hitbox.getPos() + m_hitbox.getSize() / 2));
	m_shooters.push_back(std::make_unique<Shooter_laser>(m_level, &m_position, m_hitbox.getPos() + m_hitbox.getSize() / 2));
	m_shooters.push_back(std::make_unique<Shooter_homing>(m_level, &m_position, m_hitbox.getPos() + m_hitbox.getSize() / 2));
	m_shooter = m_shooters.begin();

	m_offsetToCenter = m_hitbox.getPos() + m_hitbox.getSize() / 2;
}

void Player::update(const float frameTime_)
{
	//Update invincibility
	if (m_invincibilityTime > 0)
	{
		m_invincibilityTime -= frameTime_;
	}

	//Change animation if required
	if (m_velocity.x > 0)
	{
		if (m_currentAnimation == CurrentAnimation::LEFT)
		{
			if (spr_moveLeft->isFinished())
			{
				m_currentAnimation = CurrentAnimation::RIGHT;
				spr_moveRight->reset();
			}
		}

	}
	else if (m_velocity.x == 0)
	{
		if (m_currentAnimation == CurrentAnimation::LEFT)
		{
			if (spr_moveLeft->isFinished())
			{
				m_currentAnimation = CurrentAnimation::IDLE;
			}
		}
		else if (m_currentAnimation == CurrentAnimation::RIGHT)
		{
			if (spr_moveRight->isFinished())
			{
				m_currentAnimation = CurrentAnimation::IDLE;
			}
		}
	}
	else
	{
		if (m_currentAnimation == CurrentAnimation::RIGHT)
		{
			if (spr_moveRight->isFinished())
			{
				m_currentAnimation = CurrentAnimation::LEFT;
				spr_moveLeft->reset(2, -1);
			}
		}
	}
	
	//Update active sprites
	spr_heath->update(frameTime_);
	switch (m_currentAnimation)
	{
	case (CurrentAnimation::LEFT):
		spr_moveLeft->update(frameTime_);
		break;

	case (CurrentAnimation::RIGHT):
		spr_moveRight->update(frameTime_);
		break;
	}

	Vector2 mvm = m_velocity * frameTime_ * m_speedQuaf;

	//Save old pos
	Vector2 oldPos = m_position;

	//Try move on X
	m_position.x += mvm.x;
	if (m_level->checkStaticCollision(this))
		m_position.x = oldPos.x;

	//Try move on Y
	m_position.y += mvm.y;
	if (m_level->checkStaticCollision(this))
		m_position.y = oldPos.y;

	//Update current shooter
	(*m_shooter)->update(frameTime_);

	//Check if touched bullet
	auto collidedWith = static_pointer_cast<Projectile<class ParticleType>>(m_level->checkBulletCollision(this, TARGETS::PLAYER));
	if (collidedWith)
	{
		takeDamage(1);
		collidedWith->destroySelf();
	}
}

void Player::draw()
{
	//Draw current shooter
	(*m_shooter)->draw();

	//Draw current character sprite
	SDL_Texture* currentAnim = NULL;
	switch (m_currentAnimation)
	{
	case (CurrentAnimation::LEFT):
		currentAnim = spr_moveLeft->getSprite();
		break;

	case (CurrentAnimation::RIGHT):
		currentAnim = spr_moveRight->getSprite();
		break;

	case (CurrentAnimation::IDLE):
		currentAnim = spr_idle->getSprite();
		break;
	}

	if (m_invincibilityTime > 0)
		SDL_SetTextureColorMod(currentAnim, 100, 255, 100);

	m_core->renderTexture(currentAnim, m_position.x, m_position.y);

	if (m_invincibilityTime > 0)
		SDL_SetTextureColorMod(currentAnim, 255, 255, 255);

	//Draw health icon
	for (int i = 0; i < m_HP; ++i)
		m_core->renderTexture(spr_heath->getSprite(), 5 + i * 35, 5, 32, 32);

#ifdef SHOWHITBOXES
	drawHbox();
#endif
}

void Player::setLevel(Level* level_)
{
	m_level = level_;
	for (auto &sh : m_shooters)
		sh->setLevel(level_);
}


void Player::input_axisupd(const float scale_)
{
	m_velocity = m_core->input.getAxis() * m_maxSpd;

	if (m_velocity.x > 0)
	{
		switch (m_currentAnimation)
		{
		case (CurrentAnimation::IDLE):
			m_currentAnimation = CurrentAnimation::RIGHT;
			spr_moveRight->reset();
			break;

		case (CurrentAnimation::LEFT):
			spr_moveLeft->setDir(1);
			break;

		case (CurrentAnimation::RIGHT):
			spr_moveRight->setDir(1);
			break;
		}
		
	}
	else if (m_velocity.x == 0)
	{
		switch (m_currentAnimation)
		{
		case (CurrentAnimation::LEFT):
			spr_moveLeft->setDir(1);
			break;

		case (CurrentAnimation::RIGHT):
			spr_moveRight->setDir(-1);
			break;
		}
	}
	else
	{
		switch (m_currentAnimation)
		{
		case (CurrentAnimation::LEFT):
			spr_moveLeft->setDir(-1);
			break;

		case (CurrentAnimation::IDLE):
			m_currentAnimation = CurrentAnimation::LEFT;
			spr_moveLeft->reset(2, -1);
			break;

		case (CurrentAnimation::RIGHT):
			spr_moveRight->setDir(-1);
			break;
		}
	}

}

void Player::input_ok(const float scale_)
{
	if (scale_ == 1)
	{
		(*m_shooter)->BeginShooting();
	}
	else
	{
		(*m_shooter)->StopShooting();
	}
}

void Player::input_cancel(const float scale_)
{
	if (scale_ > 0)
	{
		bool isPrecise = (*m_shooter)->getPrecise();
		bool isShooting = (*m_shooter)->getState();
		auto old = m_shooter;
		++m_shooter;
		if (m_shooter == m_shooters.end())
			m_shooter = m_shooters.begin();
		if (isPrecise)
		{
			(*old)->QuitPrecise();
			(*m_shooter)->EnterPrecise();
		}
		if (isShooting)
		{
			(*old)->StopShooting();
			(*m_shooter)->BeginShooting();
		}
	}
}

void Player::input_shift(const float scale_)
{
	if (scale_ > 0)
	{
		m_speedQuaf = 0.4;
		(*m_shooter)->EnterPrecise();
	}
	else
	{
		m_speedQuaf = 1;
		(*m_shooter)->QuitPrecise();
	}
}

const Vector2 &Player::getShootDir()
{
	return Vector2(0, -1);
}

void Player::takeDamage(int damage_)
{
	if (m_invincibilityTime > 0)
		return;
	
	m_core->soundManager->playSfx(SOUNDS::bruh);

	m_HP--;

	//Should add delay to destruction sprite and probably move health to other class
	std::shared_ptr<Particle_expl2> ptr = std::make_shared<Particle_expl2>(m_level, Vector2(5 + 35 * m_HP + 16, 5 + 16));
	m_level->create(std::reinterpret_pointer_cast<Particle>(ptr));

	std::shared_ptr<Particle_expl3> ptr_ex = std::make_shared<Particle_expl3>(m_level, getHitbox().getPos() + m_hitbox.getSize() / 2);
	m_level->create(std::reinterpret_pointer_cast<Particle>(ptr_ex));

	setPosition(Vector2(640, 650) - m_offsetToCenter);
	m_invincibilityTime = 3;
}

void Player::setPosition(const Vector2& pos_)
{
	m_position = pos_;
}

Player::~Player()
{
	Logger::print("Huh, I was destroyed\n");
}
