#include "MainMenu.h"

MainMenu::MainMenu(int lvlId_) :
	Level(m_core->targetResolution, lvlId_)
{
	Menu* fullScreenMenu = new Menu(m_core, 120, m_size / 2 - Vector2(0, 200), Vector2(-375, 0));
	fullScreenMenu->addElement(new MenuElementAction("WINDOWED", 301));
	fullScreenMenu->addElement(new MenuElementAction("FULLSCREEN", 302));
	fullScreenMenu->createPointer();
	fullScreenMenu->linkReactor(this);

	Menu* resolutionMenu = new Menu(m_core, 120, m_size / 2 - Vector2(0, 200), Vector2(-350, 0));
	resolutionMenu->addElement(new MenuElementAction("1280x720", 401));
	resolutionMenu->addElement(new MenuElementAction("1600x900", 402));
	resolutionMenu->addElement(new MenuElementAction("1920x1080", 403));
	resolutionMenu->createPointer();
	resolutionMenu->linkReactor(this);

	Menu* settingsMenu = new Menu(m_core, 120, m_size / 2 - Vector2(0, 200), Vector2(-400, 0));
	settingsMenu->addElement(new MenuElementSubmenu("FULL SCREEN", fullScreenMenu));
	settingsMenu->addElement(new MenuElementSubmenu("RESOLUTION", resolutionMenu));
	settingsMenu->createPointer();
	settingsMenu->linkReactor(this);


	m_currentMenu = new Menu(m_core, 120, m_size / 2 - Vector2(0, 200), Vector2(-300, 0));
	m_currentMenu->addElement(new MenuElementAction("START", 101));
	m_currentMenu->addElement(new MenuElementSubmenu("SETTINGS", settingsMenu));
	m_currentMenu->addElement(new MenuElementAction("QUIT", 102));
	m_currentMenu->createPointer();
	m_currentMenu->linkReactor(this);

	resolutionMenu->setPreviousMenu(settingsMenu);
	fullScreenMenu->setPreviousMenu(settingsMenu);
	settingsMenu->setPreviousMenu(m_currentMenu);
}

void MainMenu::reactOnElementAction(MenuElement* elem_)
{
	switch (elem_->m_elementType)
	{
	case (MenuElementType::Action):
	{
		MenuElementAction* action = static_cast<MenuElementAction*>(elem_);
		switch (action->m_actionId)
		{
		case (101):
			m_returnVal.nextLvl = (int)MenuLevels::NONE + 0;
			m_isRunning = false;
			break;

		case (102):
			m_returnVal.nextLvl = -1;
			m_isRunning = false;
			break;

		case (301):
			m_core->setFS(0);
			break;

		case (302):
			m_core->setFS(2);
			break;

		case (401):
			m_core->setResolution(Vector2(1280, 720));
			break;

		case (402):
			m_core->setResolution(Vector2(1600, 900));
			break;

		case (403):
			m_core->setResolution(Vector2(1920, 1080));
			break;

		default:
			Logger::print("Menu action: " + toString<int>(action->m_actionId) + "\n");
			break;
		}
	}
		break;

	case (MenuElementType::Submenu):
	{
		MenuElementSubmenu* submenu = static_cast<MenuElementSubmenu*>(elem_);
		if (submenu->m_subMenu)
		{
			m_currentMenu->setInputEnabled(false);
			m_currentMenu = submenu->m_subMenu;
			m_currentMenu->setInputEnabled(true);
		}
	}
		break;

	case (MenuElementType::Prevmenu):
	{
		MenuElementPrevmenu* prevmenu = static_cast<MenuElementPrevmenu*>(elem_);
		if (prevmenu->m_prevMenu)
		{
			m_currentMenu->setInputEnabled(false);
			m_currentMenu = prevmenu->m_prevMenu;
			m_currentMenu->setInputEnabled(true);
			m_currentMenu->justReturned = true;
		}
		delete prevmenu;
	}
		break;
	}
}

void MainMenu::localEnterLogic()
{
	m_player->setInputEnabled(false);
	m_currentMenu->setInputEnabled(true);
}

void MainMenu::localLeaveLogic()
{
	m_currentMenu->setInputEnabled(false);
}

void MainMenu::update()
{
	float frameTime = float(m_frameTimer->m_lastFrameTime) / 1000.0f;

	m_currentMenu->update(frameTime);
}

void MainMenu::draw()
{
	m_core->fillRenderer(SDL_Color{ 0, 0, 0, 255 });
	m_currentMenu->draw();
	m_core->updateScreen();
}
