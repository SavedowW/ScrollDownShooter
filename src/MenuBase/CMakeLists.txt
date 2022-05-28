cmake_minimum_required (VERSION 3.20)
 
project(MenuBase)

set(MENUBASE_SRC_FILES
AbstractMenu.cpp
AbstractMenuItem.cpp
StringMenuItem.cpp
TextVerticalMenu.cpp
)

add_library (MenuBase ${MENUBASE_SRC_FILES})

set_property(TARGET MenuBase PROPERTY CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
target_link_libraries(MenuBase ${LINK_LIBRARIES})