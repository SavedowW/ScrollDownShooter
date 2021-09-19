#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_
#include <SDL.h>
#include <SDL_Image.h>
#include "Vector2.h"
#include <string>
#include <map>
#include "Logger.h"
#include "Sprite.h"

enum class TEXTURES
{
	player_idle = 0,
	missle1,
	Background1,
	laserFlash1,
	NONE
};

//Texture structure
struct Texture : public Sprite
{
	//Makes sure texture actually exist
	Texture(SDL_Texture* tex_)
	{
		if (tex_ == NULL)
			throw std::runtime_error("Texture does not exist!\n");

		tex = tex_;

		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	}

	//The texture and required info
	SDL_Texture* getSprite() final;
	int w, h;

	//Properly removes texture
	virtual ~Texture()
	{
		//Logger::print("Release single texture\n");
		SDL_DestroyTexture(tex);
	}

private:
	SDL_Texture* tex;
};

typedef std::shared_ptr<Texture> Texture_t;

class Core;

/**************************
*
*	Texture manager holds all textures
*	and makes sure there is only one version of each
*	texture exist and only if required
*
**************************/
class TextureManager
{
public:
	TextureManager(Core* core_);
	std::shared_ptr<Texture> getTexture(TEXTURES tex);

private:
	Core* m_core;
	std::map<TEXTURES, std::weak_ptr<Texture>> textures;
	const std::string m_files[(int)TEXTURES::NONE] = {
		"Resources/Sprites/Player/spr_idle.png",
		"Resources/Sprites/Bullets/spr_missle1.png",
		"Resources/Sprites/Backgrounds/spr_back1.png",
		"Resources/Sprites/Player/spr_laserFlash.png"
	};
};

#endif