cmake_minimum_required (VERSION 3.20)
 
project(Core)

set(CORE_SRC_FILES
AnimationManager.cpp
Core.cpp
DataSaver.cpp
Font.cpp
Input.cpp
Logger.cpp
SoundManager.cpp
Sprite.cpp
Symbol.cpp
TextManager.cpp
TextureManager.cpp
Timer.cpp
Vector2.cpp
)

add_library (Core ${CORE_SRC_FILES})

set_property(TARGET Core PROPERTY CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
target_link_libraries(Core ${LINK_LIBRARIES})