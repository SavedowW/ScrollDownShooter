#ifndef LEVEL_H_
#define LEVEL_H_
#include "Object.h"
#include "Core.h"
#include "Timer.h"
#include "Player.h"
#include "Enemies.h"

struct LevelResult
{
	int nextLvl;
};

enum class MenuLevels { MAINMENU, NONE };

//Base level class
class Level : public InputReactor
{
public:
	Level(const Vector2 &size_, int lvlId_);
	virtual void enter(); //Should be called before level - specific enter
	virtual void leave(); //Should be called before level - specific leave
	LevelResult proceed();

	//Functions to be called from Object
	virtual bool checkStaticCollision(Object* self_);
	virtual std::shared_ptr<Object> checkBulletCollision(Object* self_, TARGETS tar_);
	virtual void create(std::shared_ptr<Object> obj_);
	virtual void create(std::shared_ptr<Particle> obj_);
	virtual void destroy(Object*);
	virtual std::shared_ptr<Object> FindNearestObject(const Vector2& src_);
	virtual std::shared_ptr<Object> FindNearestAngleObject(const Vector2& src_);
	virtual Vector2 getSize();

	//Input override
	void input_quit(const float scale_) final;
	static std::shared_ptr<Player> m_player;

	virtual ~Level() = default;

	static Core* m_core;

protected:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void destroyRequested();

	Vector2 m_size;
	bool m_isRunning;
	LevelResult m_returnVal;
	int m_levelId;
	enum State {ENTER, RUN, LEAVE};

	static Timer* m_frameTimer;
};

#endif