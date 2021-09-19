#pragma once
#include "Level0.h"
#include "MainMenu.h"

Core* Object::m_core = nullptr;
Core* Level::m_core = nullptr;
Timer* Level::m_frameTimer = nullptr;
std::shared_ptr<Player> Level::m_player = nullptr;