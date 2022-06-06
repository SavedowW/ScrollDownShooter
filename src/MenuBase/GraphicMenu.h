#ifndef GRAPHIC_MENU_H_
#define GRAPHIC_MENU_H_
#include "StringMenuItem.h"

class GraphicMenu
{
public:
    GraphicMenu(std::shared_ptr<AbstractMenu> origin_);

    //To be called from level
    void update(const float frameTime_);
	void draw();

    //Menu level will call input
    void up();
    void down();
    void left();
    void right();
    void enter();
    void back();

protected:
    std::stack<std::shared_ptr<AbstractMenu>> m_menus;
    std::shared_ptr<AbstractMenu> m_currentMenu;
};

#endif