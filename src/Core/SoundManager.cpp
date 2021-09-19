#include "SoundManager.h"
#include "Core.h"

SoundManager::SoundManager(Core* core_)
{
	m_core = core_;

	m_soundList[(int)SOUNDS::swarmer_death] = loadSfx("Resources/Sounds/swarmer_death.wav", 100);
	m_soundList[(int)SOUNDS::missle1_launch] = loadSfx("Resources/Sounds/missle1_launch.wav", 30);
	m_soundList[(int)SOUNDS::missle1_destroyed] = loadSfx("Resources/Sounds/missle1_death.wav", 40);
	m_soundList[(int)SOUNDS::bruh] = loadSfx("Resources/Sounds/bruh.wav", 128);
	m_soundList[(int)SOUNDS::homingMissle_death] = loadSfx("Resources/Sounds/homingMissle_death.wav", 70);
	m_soundList[(int)SOUNDS::menu_move] = loadSfx("Resources/Sounds/menu_move.wav", 70);
	m_soundList[(int)SOUNDS::menu_select] = loadSfx("Resources/Sounds/menu_select.wav", 70);
	m_soundList[(int)SOUNDS::menu_error] = loadSfx("Resources/Sounds/menu_error.wav", 70);
}

void SoundManager::playSfx(SOUNDS sfx_)
{
	Mix_PlayChannel(-1, m_soundList[(int)sfx_].get(), 0);
}

SoundManager::~SoundManager()
{
	for (int i = 0; i < (int)SOUNDS::NONE; ++i)
	{
		Mix_FreeChunk(m_soundList->get());
	}
}

Sound_t SoundManager::loadSfx(const char* file_, int volume_)
{
	Mix_Chunk* sfx = Mix_LoadWAV(file_);
	if (sfx == NULL)
		std::cout << "SFX loading problem: " << file_ << "  |  " << Mix_GetError() << std::endl;
	else
		sfx->volume = volume_;
	return Sound_t(sfx);
}