#ifndef FONT_H_
#define FONT_H_
#include "Symbol.h"

class Font
{
public:
	Font(Core* core_, const char* s_, int fHeight_, const SDL_Color& col_);
	Font(Core* core_, TTF_Font *font_, int fHeight_, const SDL_Color& col_);
	Symbol* operator[](int id_);
	TTF_Font* m_font;
	int m_h;

private:
	Symbol* m_symbols[256];

};

#endif