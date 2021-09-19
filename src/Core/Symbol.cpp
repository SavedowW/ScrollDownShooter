#include "Symbol.h"
#include "Core.h"

Symbol::Symbol(Core* core, char ch, TTF_Font* font, const SDL_Color& col)
{
	m_tex = core->createText(&ch, col, font);
	SDL_QueryTexture(m_tex, NULL, NULL, &m_w, &m_h);
	if (m_w == 0)
		std::cout << "Failed creating symbol [" << (int)ch << "]\n";
}

SDL_Texture* Symbol::getTexture()
{
	return m_tex;
}
