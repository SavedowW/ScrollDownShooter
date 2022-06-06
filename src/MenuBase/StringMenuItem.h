#ifndef STRING_MENU_ITEM_H_
#define STRING_MENU_ITEM_H_
#include "AbstractMenuItem.h"

class StringMenuItem : public AbstractMenuItem
{
public:
    StringMenuItem(const std::string &name_,  bool isSelectable_ = true);

private:

};

#endif