#ifndef SPRITE_H_
#define SPRITE_H_
#include <SDL.h>
#include <memory>
#include <cassert>

class Sprite
{
public:
	virtual SDL_Texture* getSprite() = 0;
	virtual void update(const float frameTime_);

	virtual ~Sprite();
};

typedef std::shared_ptr<Sprite> Sprite_t;

#endif