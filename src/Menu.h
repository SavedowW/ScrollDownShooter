#ifndef MENU_H_
#define MENU_H_
#include "MenuPointer.h"

class Menu;

enum class MenuElementType {
	Action,
	Submenu,
	Prevmenu
};

class MenuElement
{
public:
	MenuElement(MenuElementType type_, const std::string &name_) :
		m_name(name_)
	{
		m_elementType = type_;
	}

	virtual ~MenuElement() = default;

	MenuElementType m_elementType;
	std::string m_name;
};

class MenuElementAction : public MenuElement
{
public:
	MenuElementAction(const std::string& name_, int actionId_) :
		MenuElement(MenuElementType::Action, name_),
		m_actionId(actionId_)
	{

	};

	int m_actionId;
};

class MenuElementSubmenu : public MenuElement
{
public:
	MenuElementSubmenu(const std::string& name_, Menu *subMenu_) :
		MenuElement(MenuElementType::Submenu, name_),
		m_subMenu(subMenu_)
	{

	};

	~MenuElementSubmenu()
	{
		delete m_subMenu;
	}

	Menu* m_subMenu;
};

class MenuElementPrevmenu : public MenuElement
{
public:
	MenuElementPrevmenu(const std::string& name_, Menu* prevMenu_) :
		MenuElement(MenuElementType::Prevmenu, name_),
		m_prevMenu(prevMenu_)
	{
	};

	Menu* m_prevMenu;
};

class ReactOnElementAction
{
public:
	virtual void reactOnElementAction(MenuElement *elem) = 0;
};

class Menu : public InputReactor
{
public:
	Menu(Core* core_, float verticalElemOffset_, const Vector2 &position_, const Vector2 &pointerOffset_, Menu* prevMenu_ = nullptr);

	void setPreviousMenu(Menu * prevMenu_);
	void addElement(MenuElement* elem_);
	void createPointer();
	void linkReactor(ReactOnElementAction* reactor_);
	void update(const float frameTime_);
	void draw();

	//Input override
	void input_up(const float scale_) override;
	void input_down(const float scale_) override;
	void input_ok(const float scale_) override;
	void input_cancel(const float scale_) override;

	bool justReturned = false;

private:
	Core* m_core;
	Vector2 m_position;

	std::unique_ptr<MenuPointer> m_menuPointer;
	std::vector<MenuElement*> m_elems;
	float m_verticalElemOffset;
	Vector2 m_pointerOffset;
	int m_currentElem = 0;

	Menu* m_prevMenu = nullptr;

	Vector2 getElementPos(int elem);

	ReactOnElementAction* m_linkedReactor = nullptr;
};

#endif