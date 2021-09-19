#include "TextManager.h"
#include <iostream>

TextManager::TextManager(Core* core_) :
	m_core(core_)
{
	m_fonts.resize((size_t)FONTS::NONE);

	//Initializing fonts
	m_fonts[(unsigned int)FONTS::TITLE] = new Font(m_core, "Resources/Fonts/good_times.ttf", 90, {255, 255, 255, 255});
	m_fonts[(unsigned int)FONTS::MENU_REGULAR] = new Font(m_core, "Resources/Fonts/good_times.ttf", 72, {255, 255, 255, 255});
}

Font* TextManager::operator[](FONTS font_) const
{
	return m_fonts[(unsigned int) font_];
}

TextManager::~TextManager()
{
	for (auto a : m_fonts)
	{
		delete a;
	}
}
