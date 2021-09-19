#ifndef ANIMATION_MANAGER_H_
#define ANIMATION_MANAGER_H_
#include "TextureManager.h"
#include <fstream>
#include "Sprite.h"

enum class ANIMATIONS
{
	player_moveLeft = 0,
	player_moveRight,
	player_rotate,
	swarmer,
	enemyShip1,
	enemyShip2,
	hit1,
	explosion1,
	explosion2,
	explosion3,
	explosion4,
	sparkle1,
	sparkle2,
	sparkle3,
	sparkle4,
	sparkle5,
	sparkle6,
	torp_autoaim,
	NONE
};

//Texture array structure
struct TextureArr
{
	//Makes sure textures actually exist
	TextureArr(SDL_Texture** t_tex, int t_amount) :
		tex(t_tex), amount(t_amount)
	{
		//assert(t_tex);

		//Sizes are based on first sprite
		SDL_QueryTexture(tex[0], NULL, NULL, &w, &h);
	}

	//Texture array and required info
	SDL_Texture** tex;
	int amount;
	int w, h;

	//Properly removes texture
	virtual ~TextureArr();
};

/**************************
*
*	Animation manager holds all texture arrays
*	and makes sure there is only one version of each
*	texture array exist and only if required
*
**************************/
class AnimationManager
{
public:
	AnimationManager(Core* t_core);
	std::shared_ptr<TextureArr> getTextureArr(ANIMATIONS t_tex);

private:
	Core* m_core;
	const std::string m_filePaths[(int)ANIMATIONS::NONE] = {
		"Resources/Animations/playerMoveLeft/",
		"Resources/Animations/playerMoveRight/",
		"Resources/Animations/playerRotate/",
		"Resources/Animations/Swarmer/",
		"Resources/Animations/EnemyShip1/",
		"Resources/Animations/EnemyShip2/",
		"Resources/Animations/Hit1/",
		"Resources/Animations/Explosion1/",
		"Resources/Animations/Explosion2/",
		"Resources/Animations/Explosion3/",
		"Resources/Animations/Explosion4/",
		"Resources/Animations/Arcane_sparkle1/01/",
		"Resources/Animations/Arcane_sparkle1/02/",
		"Resources/Animations/Arcane_sparkle1/03/",
		"Resources/Animations/Arcane_sparkle1/04/",
		"Resources/Animations/Arcane_sparkle1/05/",
		"Resources/Animations/Arcane_sparkle1/06/",
		"Resources/Animations/Torp_autoaim/",
	};
	std::map<ANIMATIONS, std::weak_ptr<TextureArr>> m_textureArrPointers;

	void preload(ANIMATIONS t_animToPreload);
	std::shared_ptr<TextureArr> m_preloadedArrs[(int)ANIMATIONS::NONE];
};

enum class LOOPMETHOD
{
	NOLOOP,
	JUMP_LOOP,
	SWITCH_DIR_LOOP
};

/**************************
*
*	Animation class
*	Call update each frame
*	Direction = 1 - direct order
*	Direction = -1 - reverse order
*
**************************/
class Animation : public Sprite
{
public:
	Animation(Core* t_core, ANIMATIONS t_textures, float t_period, LOOPMETHOD t_isLoop = LOOPMETHOD::JUMP_LOOP, int t_beginFrame = 0, int t_beginDirection = 1);
	void update(const float t_frameTime) final;
	SDL_Texture* getSprite() final;
	bool isFinished();
	void switchDir();
	void setDir(int t_dir);
	void reset(int t_beginFrame = 0, int t_beginDirection = 1);

private:
	std::shared_ptr<TextureArr> m_textures;
	Core* m_core;
	float m_period;
	float m_timePassed = 0;
	int m_currentFrame;
	int m_direction;
	LOOPMETHOD m_isLoop;
	void switchFrame();

};

typedef std::shared_ptr<Animation> Animation_t;

#endif