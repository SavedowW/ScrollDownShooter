/*
    AbstractMenu.h
    Abstract class for all menus in the project
    Every element may be both submenu or action element
*/
#ifndef ABSTRACT_MENU_H_
#define ABSTRACT_MENU_H_
#include "MenuElement.h"
#include <stdexcept>
#include <stack>

class AbstractMenu
{
public:

//Constructor
AbstractMenu();

//Menu access
MenuElement& getRoot();
MenuElement& operator[](const std::string &rootChild_);

//Input
void onBack();
void onForward();
int onConfirm();
void onReturn();

//Visuals
virtual void draw() = 0;

virtual ~AbstractMenu();

protected:
MenuElement m_root;
std::stack<MenuElement*> m_openedMenus;

};

#endif