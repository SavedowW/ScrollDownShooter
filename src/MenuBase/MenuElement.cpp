#include "MenuElement.h"

MenuElement::MenuElement() : MenuElement("root")
{

}

MenuElement::MenuElement(const std::string &name_)
{
    m_name = name_;
}

MenuElement& MenuElement::operator[](const std::string &child_)
{
    for(auto &elem : m_elements)
    {
        if (elem.get_name() == child_)
            return elem;
    }

    m_elements.push_back(MenuElement(child_));
    return *m_elements.end();
}

MenuElement& MenuElement::operator[](int child_)
{
    return m_elements[child_];
}

const std::string& MenuElement::get_name() const
{
    return m_name;
}

MenuElement& MenuElement::setSelectedElement(int selectedElement_)
{
    m_selectedElement = selectedElement_;
    clampSelection();
    return *this;
}

void MenuElement::onBack()
{
    setSelectedElement(m_selectedElement - 1);
}

void MenuElement::onForward()
{
    setSelectedElement(m_selectedElement + 1);
}

MenuElement* MenuElement::onConfirm()
{
    if (hasChildren())
        return &m_elements[m_selectedElement];
    else
        return this;
}

MenuElement& MenuElement::setAction(int action_)
{
    m_action = action_;
    return *this;
}

int MenuElement::getAction() const
{
    return m_action;
}

bool MenuElement::hasChildren() const
{
    return m_elements.size() > 0;
}

int MenuElement::childrenAmount() const
{
    return m_elements.size();
}

int MenuElement::getSelected()
{
    return m_selectedElement;
}

void MenuElement::clampSelection()
{
    if (m_selectedElement < 0)
    {
    m_selectedElement = 0;
    }
    else if (m_selectedElement > m_elements.size() - 1)
    {
        m_selectedElement = m_elements.size() - 1;
    }
}