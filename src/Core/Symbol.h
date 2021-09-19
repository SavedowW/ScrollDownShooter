#ifndef SYMBOL_H_
#define SYMBOL_H_
#include <SDL.h>
#include <SDL_ttf.h>

class Core;

class Symbol
{
public:
	Symbol(Core* core, char ch, TTF_Font* font, const SDL_Color& col);
	SDL_Texture* getTexture();
	int m_w, m_h;

private:
	SDL_Texture* m_tex;
};

#endif