#include "StaticInit.h"

int SDL_main(int argc, char* args[])
{
	std::cout << "Imagine a new feature was implemented\n";

	//Init core
	Core* core = new Core;
	if (!core->init())
	{
		std::system("pause");
		return 1;
	}
	Object::m_core = core;
	Level::m_core = core;

	//Init player
	Level::m_player = std::shared_ptr<Player>(new Player(nullptr));

	//Init levels
	Level* levels[2];
	levels[(unsigned int)MenuLevels::MAINMENU] = new MainMenu((unsigned int)MenuLevels::MAINMENU);;
	levels[(unsigned int)MenuLevels::NONE + 0] = new Level0((unsigned int)MenuLevels::NONE + 0);
	LevelResult levelRet = {0};

	//Main level loop
	while (levelRet.nextLvl != -1)
	{
		levels[levelRet.nextLvl]->enter();
		levelRet = levels[levelRet.nextLvl]->proceed();
	}

	//Clean up everything
	for (Level* lvl : levels)
	{
		if (lvl)
			delete lvl;
	}
	Level::m_player.reset();
	core->close();
	delete core;
	return 0;
}