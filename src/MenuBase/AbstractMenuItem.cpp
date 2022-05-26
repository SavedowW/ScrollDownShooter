#include "AbstractMenuItem.h"

AbstractMenuItem::AbstractMenuItem(const std::string &name_, bool isSelectable_) :
    m_name(name_),
    m_isSelectable(isSelectable_)
{

}

std::string AbstractMenuItem::getName()
{
    return m_name;
}

bool AbstractMenuItem::isSelectable()
{
    return m_isSelectable;
}

int AbstractMenuItem::enter()
{
    return 0;
}

int AbstractMenuItem::select()
{
    return 0;
}

int AbstractMenuItem::forward()
{
    return 0;
}

int AbstractMenuItem::backward()
{
    return 0;
}

int AbstractMenuItem::previous()
{
    return 0;
}

int AbstractMenuItem::next()
{
    return 0;
}

int AbstractMenuItem::back()
{
    return 0;
}

AbstractMenuItem::~AbstractMenuItem()
{
    std::cout << "Deleted " << m_name << std::endl;
}