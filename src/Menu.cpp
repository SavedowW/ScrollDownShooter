#include "Menu.h"

Menu::Menu(Core* core_, float verticalElemOffset_, const Vector2& position_, const Vector2& pointerOffset_, Menu* prevMenu_) :
	InputReactor(&core_->input),
	m_core(core_),
	m_position(position_),
	m_verticalElemOffset(verticalElemOffset_),
	m_prevMenu(prevMenu_),
	m_pointerOffset(pointerOffset_)
{
	m_core->input.subscribe(EVENTS::UP, dynamic_cast<InputReactor*>(this));
	m_core->input.subscribe(EVENTS::DOWN, dynamic_cast<InputReactor*>(this));
	m_core->input.subscribe(EVENTS::OK, dynamic_cast<InputReactor*>(this));
	m_core->input.subscribe(EVENTS::CANCEL, dynamic_cast<InputReactor*>(this));
}

void Menu::setPreviousMenu(Menu* prevMenu_)
{
	m_prevMenu = prevMenu_;
}

void Menu::addElement(MenuElement* elem_)
{
	m_elems.push_back(elem_);
}

void Menu::createPointer()
{
	m_menuPointer = std::unique_ptr<MenuPointer>(new MenuPointer(m_core, getElementPos(0) + m_pointerOffset));
}

void Menu::linkReactor(ReactOnElementAction* reactor_)
{
	m_linkedReactor = reactor_;
}

void Menu::update(const float frameTime_)
{
	m_menuPointer->update(frameTime_);
}

void Menu::draw()
{
	for (int i = 0; i < m_elems.size(); ++i)
	{
		m_core->quickDrawText(m_elems[i]->m_name.c_str(), m_position + Vector2(0, m_verticalElemOffset * i), FONTS::MENU_REGULAR, Core::ALIGN_VER::TOP, Core::ALIGN_HOR::CENTER);
	}
	m_menuPointer->draw();
}

void Menu::input_up(const float scale_)
{
	if (scale_ <= 0)
		return;

	m_core->soundManager->playSfx(SOUNDS::menu_move);
	m_currentElem--;
	if (m_currentElem < 0)
		m_currentElem = m_elems.size() - 1;

	m_menuPointer->moveTo(getElementPos(m_currentElem) + m_pointerOffset);
}

void Menu::input_down(const float scale_)
{
	if (scale_ <= 0)
		return;

	m_core->soundManager->playSfx(SOUNDS::menu_move);
	m_currentElem++;
	if (m_currentElem >= m_elems.size())
		m_currentElem = 0;

	m_menuPointer->moveTo(getElementPos(m_currentElem) + m_pointerOffset);
}

void Menu::input_ok(const float scale_)
{
	if (!m_linkedReactor || scale_ <= 0)
		return;

	m_core->soundManager->playSfx(SOUNDS::menu_select);
	m_linkedReactor->reactOnElementAction(m_elems[m_currentElem]);
}

void Menu::input_cancel(const float scale_)
{
	if (!m_linkedReactor || scale_ <= 0)
		return;

	if (justReturned)
	{
		justReturned = false;
		return;
	}

	m_core->soundManager->playSfx(SOUNDS::menu_select);
	m_linkedReactor->reactOnElementAction(new MenuElementPrevmenu("Previous menu", m_prevMenu));
}

Vector2 Menu::getElementPos(int elem)
{
	assert(m_elems.size() > 0);
	return Vector2(m_position.x, m_position.y + elem * m_verticalElemOffset);
}
