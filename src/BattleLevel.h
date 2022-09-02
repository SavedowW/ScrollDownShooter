#ifndef BATTLE_LEVEL_H_
#define BATTLE_LEVEL_H_
#include "Level.h"
#include "Background.h"

//Battle level base class
class BattleLevel : public Level
{
public:
	BattleLevel(const Vector2& size_, int lvlId_);

	//Functions for objects
	bool checkStaticCollision(Object* self_) override;
	std::shared_ptr<Object> checkBulletCollision(Object* self_, TARGETS tar_) override;
	void create(std::shared_ptr<Object> obj_) override;
	void destroy(Object* obj_) override;
	std::shared_ptr<Object> FindNearestObject(const Vector2& src_) override;
	std::shared_ptr<Object> FindNearestAngleObject(const Vector2& src_) override;

	//Input override
	void input_ctrl(const float scale_) override;

	virtual Vector2 getSpawnPoint();

	virtual ~BattleLevel() = default;

protected:
	const Vector2 spawnPoint = m_core->targetResolution / 2 + Vector2(-50, 200);

	void update() override;
	void draw() override;
	void destroyRequested() override;
	void localEnterLogic() override;
	void localLeaveLogic() override;

	//Player and enemies
	//Shared because also contains player pointer, but actually has sole ownership of everything else
	std::vector<std::shared_ptr<Object>> m_actors;

	//Bullets, missles, etc
	std::vector<std::shared_ptr<Object>> m_projectiles;

	//Temporary effects without interactions
	std::vector<std::shared_ptr<Object>> m_particles;

	std::list<Object*> m_toBeDestroyed;

	std::unique_ptr<Background> m_background;

	float m_spawnTimer = 0;
};

#endif