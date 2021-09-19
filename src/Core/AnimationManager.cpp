#include "Core.h"
#include "AnimationManager.h"

AnimationManager::AnimationManager(Core* t_core) :
	m_core(t_core)
{
	//Init preloaded textures arr
	for (int i = 0; i < (int)ANIMATIONS::NONE; ++i)
	{
		m_preloadedArrs[i] = nullptr;
	}

	//Load preloading textures
	preload(ANIMATIONS::hit1);
	preload(ANIMATIONS::explosion1);
	preload(ANIMATIONS::explosion2);
	preload(ANIMATIONS::explosion3);
	preload(ANIMATIONS::explosion4);

	//Init pointers to texture arrays
	for (int i = 0; i < int(ANIMATIONS::NONE); ++i)
	{
		m_textureArrPointers[ANIMATIONS(i)] = std::weak_ptr<TextureArr>();
	}
}

std::shared_ptr<TextureArr> AnimationManager::getTextureArr(ANIMATIONS t_tex)
{
	if (m_preloadedArrs[(int)t_tex] != nullptr)
	{
		//Logger::print("Texture arr is preloaded\n");
		return m_preloadedArrs[(int)t_tex];
	}
	else if (m_textureArrPointers[t_tex].expired())
	{
		//Logger::print("Texture arr does not exist, creating new\n");

		//Parse data.txt
		std::ifstream loadData(m_filePaths[(int)t_tex] + "data.txt");
		std::string prefix;
		int ibegin, iend;
		loadData >> prefix >> ibegin >> iend;
		int amount = iend - ibegin + 1;

		//Load all textures
		SDL_Texture** texs = new SDL_Texture * [amount];
		for (int i = ibegin; i <= iend; ++i)
		{
			texs[i - ibegin] = m_core->loadTexture((m_filePaths[(int)t_tex] + prefix + toString<int>(i) + ".png").c_str());
		}

		//Create TextureArray with loaded textures
		std::shared_ptr<TextureArr> reqElem(new TextureArr(texs, amount));
		m_textureArrPointers[t_tex] = reqElem;
		return reqElem;
	}
	else
	{
		//Logger::print("Texture arr already exist\n");
		return m_textureArrPointers[t_tex].lock();
	}
}

void AnimationManager::preload(ANIMATIONS t_animToPreload)
{
	m_preloadedArrs[(int)t_animToPreload] = std::shared_ptr<TextureArr>(getTextureArr(t_animToPreload));
	m_textureArrPointers[t_animToPreload] = m_preloadedArrs[(int)t_animToPreload];
}

//Properly release all textures
TextureArr::~TextureArr()
{
	//Logger::print("Release " + intToString(amount) + " textures\n");
	for (int i = 0; i < amount; ++i)
		SDL_DestroyTexture(tex[i]);
	delete[] tex;
}

Animation::Animation(Core* t_core, ANIMATIONS t_textures, float t_period, LOOPMETHOD t_isLoop, int t_beginFrame, int t_beginDirection) :
	m_core(t_core),
	m_period(t_period),
	m_isLoop(t_isLoop),
	m_currentFrame(t_beginFrame),
	m_direction(t_beginDirection)
{
	m_textures = m_core->animationManager->getTextureArr(t_textures);
}


void Animation::update(const float t_frameTime)
{
	//Skips frames if required
	m_timePassed += t_frameTime;
	while (m_timePassed > m_period)
	{
		m_timePassed -= m_period;
		switchFrame();
	}
}

SDL_Texture* Animation::getSprite()
{
	return m_textures->tex[m_currentFrame];
}

bool Animation::isFinished()
{
	switch (m_isLoop)
	{
	case (LOOPMETHOD::NOLOOP):
		if (m_direction > 0)
			return m_currentFrame == m_textures->amount - 1;
		else
			return !m_currentFrame;
		break;

	case (LOOPMETHOD::JUMP_LOOP):
		[[fallthrough]];

	case (LOOPMETHOD::SWITCH_DIR_LOOP):
		return false;
		break;
	}
}

void Animation::switchDir()
{
	m_direction *= -1;
}

void Animation::setDir(int t_dir)
{
	assert(t_dir == 1 || t_dir == -1);

	m_direction = t_dir;
}

void Animation::reset(int t_beginFrame, int t_beginDirection)
{
	m_currentFrame = t_beginFrame;
	m_direction = t_beginDirection;
	m_timePassed = 0;
}

void Animation::switchFrame()
{
	switch (m_isLoop)
	{
	case (LOOPMETHOD::NOLOOP):
		if (isFinished())
			return;
		m_currentFrame += m_direction;
		break;

	case (LOOPMETHOD::JUMP_LOOP):
		m_currentFrame += m_direction;
		if (m_currentFrame >= m_textures->amount)
			m_currentFrame = 0;
		else if (m_currentFrame < 0)
			m_currentFrame = m_textures->amount - 1;
		break;

	case (LOOPMETHOD::SWITCH_DIR_LOOP):
		m_currentFrame += m_direction;
		if (m_currentFrame >= m_textures->amount)
		{
			switchDir();
			m_currentFrame += 2 * m_direction;
		}
		else if (m_currentFrame < 0)
		{
			switchDir();
			m_currentFrame += 2 * m_direction;
		}
		break;
	}
}
