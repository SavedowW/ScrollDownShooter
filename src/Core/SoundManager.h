#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_Mixer.h>
#include "Vector2.h"
#include <string>
#include <map>
#include "Logger.h"

class Core;

enum class SOUNDS
{
	swarmer_death,
	missle1_launch,
	missle1_destroyed,
	homingMissle_death,
	bruh,
	menu_move,
	menu_select,
	menu_error,
	NONE
};

typedef std::unique_ptr<Mix_Chunk> Sound_t;

class SoundManager
{
public:
	SoundManager(Core *core_);
	void playSfx(SOUNDS sfx_);
	~SoundManager();

private:
	Sound_t loadSfx(const char* file_, int volume_);
	Sound_t m_soundList[(int)SOUNDS::NONE];
	Core* m_core;

};

#endif