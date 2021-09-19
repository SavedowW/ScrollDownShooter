#ifndef BACKGROUND_H_
#define BACKGROUND_H_
#include "Core.h"

//This class manages the infinitely scrolling background
class Background
{
public:
	Background(Core* core_);
	void update(float frameTime_);
	void draw();

private:
	Texture_t m_back;
	float m_pos[2];
	Core* m_core;
	const float m_spd = 200;

	float scaling;
	float m_sizeX;
	float m_sizeY;
};

#endif