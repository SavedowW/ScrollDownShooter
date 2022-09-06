#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_
#include "Level.h"
#include "Menu.h"

class MainMenu : public Level, public ReactOnElementAction
{
public:
	MainMenu(int lvlId_);
	void enter() override;
	void leave() override;

	void reactOnElementAction(MenuElement* elem_) override;

protected:
	virtual void update() override;
	virtual void draw() override;

	Menu* m_currentMenu = nullptr;
};

#endif