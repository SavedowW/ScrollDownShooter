#include "AbstractMenu.h"

AbstractMenu::AbstractMenu()
{
    m_openedMenus.push(&m_root);
}

MenuElement& AbstractMenu::getRoot()
{
    return m_root;
}

MenuElement& AbstractMenu::operator[](const std::string &rootChild_)
{
    return m_root[rootChild_];
}

void AbstractMenu::onBack()
{
    m_openedMenus.top()->onBack();
}

void AbstractMenu::onForward()
{
    m_openedMenus.top()->onForward();
}

int AbstractMenu::onConfirm()
{
    MenuElement* res = m_openedMenus.top()->onConfirm();
    if (res->hasChildren())
    {
        m_openedMenus.push(res);
        return 0;
    }
    else
        return res->getAction();
}

void AbstractMenu::onReturn()
{
    if (m_openedMenus.size() <= 1)
        throw std::runtime_error("Cannot leave top-level menu!");

    m_openedMenus.pop();
}

AbstractMenu::~AbstractMenu()
{

}