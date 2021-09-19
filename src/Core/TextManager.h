#ifndef TEXT_MANAGER_H_
#define TEXT_MANAGER_H_
#include "Font.h"
#include <vector>

class Core;

enum class FONTS
{
	TITLE = 0,
	MENU_REGULAR,
	NONE
};

class TextManager
{
public:
	TextManager(Core* core_);
	Font* operator[](FONTS font_) const;

	~TextManager();

private:
	Core* m_core;
	std::vector<Font*> m_fonts;

};

#endif