#include <string>
#include <memory>
#include <iostream>

class AbstractMenuItem
{
public:
    AbstractMenuItem(const std::string &name_,  bool isSelectable_ = true);
    virtual std::string getName();
    virtual bool isSelectable();
    virtual int forward();
    virtual int enter();
    virtual int select();
    virtual int backward();
    virtual int previous();
    virtual int next();
    virtual int back();
    virtual ~AbstractMenuItem();


protected:
    std::string m_name;
    bool m_isSelectable;
};