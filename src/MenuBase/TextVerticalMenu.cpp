#include "TextVerticalMenu.h"

TextVerticalMenu::TextVerticalMenu(std::shared_ptr<AbstractMenu> origin_)
{
    m_menus.push(origin_);
    m_currentMenu = m_menus.top();
}

void TextVerticalMenu::display()
{
    std::cout << m_menus.top()->getName() << std::endl;
    int size = m_currentMenu->size();
    int selected = m_currentMenu->getSelected();
    for (int i = 0; i < size; ++i)
    {
        if (i == selected)
            std::cout << ">";
        std::cout << (*m_currentMenu)[i].m_originalPtr->getName() << std::endl;
    }
}

void TextVerticalMenu::up()
{
    m_currentMenu->previous();
}

void TextVerticalMenu::down()
{
    m_currentMenu->next();
}

void TextVerticalMenu::left()
{
    m_currentMenu->backward();
}

void TextVerticalMenu::right()
{
    m_currentMenu->forward();
}

void TextVerticalMenu::enter()
{
    int result = m_currentMenu->enter();
    if (result == 1)
    {
        std::shared_ptr<AbstractMenuItem> item = (*m_currentMenu)[m_currentMenu->getSelected()].m_originalPtr;
        std::shared_ptr<AbstractMenu> nextMenu = std::dynamic_pointer_cast<AbstractMenu>(item);
        m_menus.push(nextMenu);
        m_currentMenu = nextMenu;
    }
}

void TextVerticalMenu::back()
{
    int result = m_currentMenu->back();
    if (result == 1)
        if (m_menus.size() > 1)
        {
            m_menus.pop();
            m_currentMenu = m_menus.top();
        }
}