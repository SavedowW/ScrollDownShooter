#include "Level.h"

Level::Level(const Vector2& size_, int lvlId_) :
	InputReactor(&m_core->input),
	m_size(size_),
	m_levelId(lvlId_)
{
	if (m_frameTimer == nullptr)
		m_frameTimer = new Timer;
	m_core->input.subscribe(EVENTS::QUIT, dynamic_cast<InputReactor*>(this));

	setInputEnabled(false);
}

//Runs when enter the function
void Level::enter()
{
	m_isRunning = true;
	m_returnVal = { -1 };
	m_frameTimer->begin();
	m_player->setLevel(this);
	setInputEnabled(true);

	localEnterLogic();
}

void Level::leave()
{
	setInputEnabled(false);

	localLeaveLogic();
}

LevelResult Level::proceed()
{
	while (m_isRunning)
	{
		m_core->input.handleInput();

		update();
		draw();
		destroyRequested();

		m_frameTimer->getPassedMS();
	}

	leave();

	return m_returnVal;
}

bool Level::checkStaticCollision(Object* self_)
{
	throw std::runtime_error("checkStaticCollision() should not be called without overriding");
}

std::shared_ptr<Object> Level::checkBulletCollision(Object* self_, TARGETS tar_)
{
	throw std::runtime_error("checkBulletCollision() should not be called without overriding");
	return nullptr;
}

void Level::create(std::shared_ptr<Object> obj_)
{
	throw std::runtime_error("create() should not be called without overriding");
}

void Level::destroy(Object*)
{
	throw std::runtime_error("destroy() should not be called without overriding");
}

std::shared_ptr<Object> Level::FindNearestObject(const Vector2& src_)
{
	throw std::runtime_error("FindNearestObject() should not be called without overriding");
	return nullptr;
}

std::shared_ptr<Object> Level::FindNearestAngleObject(const Vector2& src_)
{
	throw std::runtime_error("FindNearestAngleObject() should not be called without overriding");
	return nullptr;
}

//Override for specific logic of enter
void Level::localEnterLogic()
{
	
}

void Level::localLeaveLogic()
{
}

void Level::destroyRequested()
{
}

Vector2 Level::getSize()
{
	return m_size;
}

void Level::input_quit(const float scale_)
{
	m_returnVal = { -1 };
	m_isRunning = false;
}

Level::~Level()
{
}
