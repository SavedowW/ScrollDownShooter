#ifndef MENU_POINTER_H_
#define MENU_POINTER_H_
#include "Core.h"

class MenuPointer
{
public:
	MenuPointer(Core* core_, const Vector2 &position_);
	void moveTo(const Vector2 &target_);
	void update(const float frameTime_);
	void draw();

private:
	Core* m_core;
	Vector2 m_position;
	Vector2 m_target;
	Animation_t m_anim;

	const float maxSpd = 1000;
	const float smoothDist = 50;
};

#endif