#include "BattleLevel.h"


BattleLevel::BattleLevel(const Vector2& size_, int lvlId_) :
	Level(size_, lvlId_),
	m_background(std::make_unique<Background>(m_core))
{

	m_core->input.subscribe(EVENTS::CTRL, this);
}

bool BattleLevel::checkStaticCollision(Object* self_)
{
	Hitbox hBox = self_->getHitbox();
	if (hBox.x < 0 || hBox.x + hBox.w > m_size.x ||
		hBox.y < 0 || hBox.y + hBox.h > m_size.y)
		return true;
	return false;
}

std::shared_ptr<Object> BattleLevel::checkBulletCollision(Object* self_, TARGETS tar_)
{
	Hitbox selfHBox = self_->getHitbox();
	for (auto obj : m_projectiles)
	{
		Hitbox bullHBox = obj->getHitbox();
		if (tar_ == std::static_pointer_cast<Projectile<class ParticleType>>(obj)->m_target && selfHBox.isCollideWith(bullHBox))
			return obj;
	}
	return nullptr;
}

void BattleLevel::create(std::shared_ptr<Object> obj_)
{
	assert(obj_.get());

	switch (obj_->m_tag)
	{
	case (OBJ_TAGS::OBJECT):
		m_actors.push_back(obj_);
		break;

	case (OBJ_TAGS::BULLET):
		m_projectiles.push_back(obj_);
		break;

	case (OBJ_TAGS::PARTICLE):
		m_particles.push_back(obj_);
		break;

	default:
		throw std::runtime_error("Trying to create object without a tag");
		break;
	}
}

/*
* Since this function is usually called from
* update function, we cannot simply destroy requested object
* because it can easily break the loop. So that heavy
* function simply saves objects which are "to be destroyed" and
* removes them after rendering loop
*/
void BattleLevel::destroy(Object* obj_)
{
	assert(obj_);

	if (obj_->m_destroyed)
		return;

	obj_->m_destroyed = true;
	m_toBeDestroyed.push_back(obj_);
}

std::shared_ptr<Object> BattleLevel::FindNearestObject(const Vector2& src_)
{
	float dist = 0;
	std::shared_ptr<Object> avaibleObject = nullptr;
	for (auto& obj : m_actors)
	{
		if (obj == m_player)
			continue;

		if (!avaibleObject)
		{
			dist = (obj->getPosition() - src_).getSqLen();
			avaibleObject = obj;
		}
		else
		{
			float tempDist = (obj->getPosition() - src_).getSqLen();
			if (tempDist < dist)
			{
				dist = tempDist;
				avaibleObject = obj;
			}
		}
	}
	return avaibleObject;
}

std::shared_ptr<Object> BattleLevel::FindNearestAngleObject(const Vector2& src_)
{
	float angle = 0;
	std::shared_ptr<Object> avaibleObject = nullptr;
	for (auto& obj : m_actors)
	{
		if (obj == m_player)
			continue;

		if (!avaibleObject)
		{
			Vector2 local = obj->getPosition() - src_;
			angle = abs(atan2(-local.x, -local.y));
			avaibleObject = obj;
		}
		else
		{
			Vector2 local = obj->getPosition() - src_;
			float tempAngle = abs(atan2(-local.x, -local.y));
			if (tempAngle < angle)
			{
				angle = tempAngle;
				avaibleObject = obj;
			}
		}
	}
	return avaibleObject;
}

void BattleLevel::input_ctrl(const float scale_)
{
	m_returnVal = {(int)MenuLevels::MAINMENU};
	m_isRunning = false;
}

Vector2 BattleLevel::getSpawnPoint()
{
	return spawnPoint;
}

void BattleLevel::update()
{
	float frameTime = float(m_frameTimer->m_lastFrameTime) / 1000.0f;

	m_background->update(frameTime);

	for (auto& ptr : m_actors)
	{
		std::shared_ptr<Object> proofOfExistence(ptr);
		ptr->update(frameTime);
	}
	for (auto& ptr : m_projectiles)
	{

		ptr->update(frameTime);
	}
	for (auto& ptr : m_particles)
	{

		ptr->update(frameTime);
	}

	m_spawnTimer += frameTime;
	if (m_spawnTimer > 3)
	{
		for (int i = 0; i < 3 ;++i)
			create(std::shared_ptr<Object>(new Swarmer(this, m_player.get(), Vector2(rand() % int(m_size.x), -80))));
		create(std::shared_ptr<Object>(new EnemyShip1<Shooter_sparkler_targeted>(this, m_player.get(), Vector2(0, -80), Vector2(0, 1))));
		create(std::shared_ptr<Object>(new EnemyShip1<Shooter_sparkler_targeted>(this, m_player.get(), Vector2(m_size.x - 93, -80), Vector2(0, 1))));
		m_spawnTimer = 0;
	}
}

void BattleLevel::draw()
{
	m_core->fillRenderer(SDL_Color{ 255, 255, 255, 255 });
	m_background->draw();

	for (auto& ptr : m_projectiles)
	{
		ptr->draw();
	}
	for (auto& ptr : m_actors)
	{
		ptr->draw();
	}
	for (auto& ptr : m_particles)
	{
		ptr->draw();
	}

	m_core->updateScreen();
}

void BattleLevel::destroyRequested()
{
	int size = m_toBeDestroyed.size();
	int counter = size;

	while (counter > 0)
	{
		switch ((*(m_toBeDestroyed.begin()))->m_tag)
		{
		case (OBJ_TAGS::OBJECT):
			for (int i = 0; i < m_actors.size(); ++i)
			{
				if (m_actors[i].get() == *m_toBeDestroyed.begin())
					m_actors.erase(m_actors.begin() + i);
			}
			break;

		case (OBJ_TAGS::BULLET):
			for (int i = 0; i < m_projectiles.size(); ++i)
			{
				if (m_projectiles[i].get() == *m_toBeDestroyed.begin())
					m_projectiles.erase(m_projectiles.begin() + i);
			}
			break;

		case (OBJ_TAGS::PARTICLE):
			for (int i = 0; i < m_particles.size(); ++i)
			{
				if (m_particles[i].get() == *m_toBeDestroyed.begin())
					m_particles.erase(m_particles.begin() + i);
			}
			break;

		default:
			throw std::runtime_error("Trying to destroy object without a tag");
			break;
		}
		m_toBeDestroyed.erase(m_toBeDestroyed.begin());
		counter--;
	}
}

void BattleLevel::localEnterLogic()
{
	m_actors.push_back(m_player);
	m_player->setPosition(getSpawnPoint());
	m_player->setInputEnabled(true);
}

void BattleLevel::localLeaveLogic()
{
	m_actors.clear();
	m_projectiles.clear();
	m_particles.clear();
	m_toBeDestroyed.clear();
}
