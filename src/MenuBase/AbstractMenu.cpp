#include "AbstractMenu.h"

ItemHolderPtr &ItemHolderPtr::operator=(const std::string &rhs)
{
    m_originalPtr.reset();
    std::shared_ptr<StringMenuItem> newItem = std::make_shared<StringMenuItem>(rhs);
    m_originalPtr = std::dynamic_pointer_cast<AbstractMenuItem>(newItem);
    return *this;
}

ItemHolderPtr &ItemHolderPtr::operator=(std::shared_ptr<AbstractMenu> &menu_)
{
    m_originalPtr = menu_;
    return *this;
}

AbstractMenu::AbstractMenu(const std::string &name_,  bool isSelectable_) :
    AbstractMenuItem(name_, isSelectable_)
{

}

void AbstractMenu::resize(int size_)
{
    m_items.resize(size_);
}

int AbstractMenu::size()
{
    return m_items.size();
}

int AbstractMenu::getSelected()
{
    return m_selected;
}

const std::string &getMenuName()
{
    
}

ItemHolderPtr AbstractMenu::operator[](int id_)
{
    return ItemHolderPtr{m_items[id_], this};
}

int AbstractMenu::forward()
{
    if (m_selected >= 0 && m_selected < m_items.size())
        m_items[m_selected]->forward();
    
    return 0;
}

int AbstractMenu::enter()
{
    if (m_selected >= 0 && m_selected < m_items.size())
    {
        return m_items[m_selected]->select();
    }

    return 0;
}

int AbstractMenu::select()
{
    return 1;
}

int AbstractMenu::backward()
{
    if (m_selected >= 0 && m_selected < m_items.size())
        m_items[m_selected]->backward();
    
    return 0;
}

int AbstractMenu::previous()
{
    m_selected--;
    if (m_selected < 0)
        m_selected = m_items.size() - 1;

    if (!m_items[m_selected]->isSelectable())
        previous();

    return 0;
}

int AbstractMenu::next()
{
    m_selected++;
    if (m_selected >= m_items.size())
        m_selected = 0;

    if (!m_items[m_selected]->isSelectable())
        next();

    return 0;
}

int AbstractMenu::back()
{
    return 1;
}