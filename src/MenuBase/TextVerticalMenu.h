#include "AbstractMenu.h"
#include <stack>

class TextVerticalMenu
{
public:
    TextVerticalMenu(std::shared_ptr<AbstractMenu> origin_);
    void display();

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