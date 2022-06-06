#include "StringMenuItem.h"

class GraphicMenu
{
public:
    GraphicMenu(std::shared_ptr<AbstractMenu> origin_);

    //To be called from level
    void update(const float frameTime_);
	void draw();

    //
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