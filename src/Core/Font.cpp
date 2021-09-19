#include "Font.h"
#include "Core.h"

Font::Font(Core* core_, const char* s_, int fHeight_, const SDL_Color& col_)
{
	m_font = core_->createFont(s_, fHeight_);
	for (int i = 1; i < 256; i++)
	{
		m_symbols[i] = new Symbol(core_, char(i), m_font, col_);
	}
	m_h = TTF_FontLineSkip(m_font);
}

Font::Font(Core* core_, TTF_Font* font_, int fHeight_, const SDL_Color& col_)
{
	m_font = font_;
	for (int i = 1; i < 256; i++)
	{
		m_symbols[i] = new Symbol(core_, char(i), m_font, col_);
	}
	m_h = fHeight_;
}

Symbol* Font::operator[](int id_)
{
	assert(id_ != 0);
	return m_symbols[id_];
}
