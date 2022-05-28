#include "StringMenuItem.h"
#include <vector>

class AbstractMenu;

struct ItemHolderPtr
{
    std::shared_ptr<AbstractMenuItem> &m_originalPtr;
    AbstractMenu *m_menu;
    ItemHolderPtr &operator=(const std::string &rhs);
    ItemHolderPtr &operator=(std::shared_ptr<AbstractMenu> &menu_);
};

class AbstractMenu : public AbstractMenuItem
{
public:
    AbstractMenu(const std::string &name_,  bool isSelectable_ = true);
    const std::string &getMenuName();
    void resize(int size_);
    int size();
    int getSelected();
    ItemHolderPtr operator[](int id_);
    int forward() override;
    int enter() override;
    int select() override;
    int backward() override;
    int previous() override;
    int next() override;
    int back() override;

protected:
    std::vector<std::shared_ptr<AbstractMenuItem>> m_items;
    int m_selected = 0;

};
