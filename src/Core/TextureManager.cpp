#include "TextureManager.h"
#include "Core.h"

TextureManager::TextureManager(Core* core_) :
	m_core(core_)
{
	//Init pointers to textures
	for (int i = 0; i < int(TEXTURES::NONE); ++i)
	{
		textures[TEXTURES(i)] = std::weak_ptr<Texture>();
	}
}

std::shared_ptr<Texture> TextureManager::getTexture(TEXTURES tex_)
{
	if (textures[tex_].expired())
	{
		//Logger::print("Texture does not exist, creating new\n");
		std::shared_ptr<Texture> reqElem(new Texture(m_core->loadTexture(m_files[(int)tex_].c_str())));
		textures[tex_] = reqElem;
		return reqElem;
	}
	else
	{
		//Logger::print("Texture already exist\n");
		return textures[tex_].lock();
	}
}

SDL_Texture* Texture::getSprite()
{
	return tex;
}
