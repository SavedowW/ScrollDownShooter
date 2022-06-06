#include "GraphicMenu.h"

GraphicMenu::GraphicMenu(std::shared_ptr<AbstractMenu> origin_)
{
    m_menus.push(origin_);
    m_currentMenu = m_menus.top();
}

void GraphicMenu::update(const float frameTime_)
{

}

void GraphicMenu::draw()
{

}

void GraphicMenu::up()
{

}

void GraphicMenu::down()
{

}

void GraphicMenu::left()
{

}

void GraphicMenu::right()
{

}

void GraphicMenu::enter()
{

}

void GraphicMenu::back()
{

}
