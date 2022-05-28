cmake_minimum_required (VERSION 3.20)
 
set(PROJECT_NAME ScrollDownShooter)

project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_gfx REQUIRED)

set(SRC_FILES 
main.cpp
Background.cpp
BattleActor.cpp
BattleLevel.cpp
Enemies.cpp
Level.cpp
Level0.cpp
Object.cpp
Particle.cpp
Player.cpp
Projectiles.cpp
Shooter.cpp
)

set(INCLUDE_DIRS
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/Core
${CMAKE_CURRENT_LIST_DIR}/MenuBase
${SDL2_INCLUDE_DIRS}
${SDL2_IMAGE_INCLUDE_DIRS}
${SDL2_TTF_INCLUDE_DIRS}
${SDL2_MIXER_INCLUDE_DIRS}
${SDL2_GFX_INCLUDE_DIRS}
)

set(LINK_LIBRARIES
${SDL2_LIBRARIES}
${SDL2_IMAGE_LIBRARIES}
${SDL2_TTF_LIBRARIES}
${SDL2_MIXER_LIBRARIES}
${SDL2_GFX_LIBRARIES}
)

add_executable (${PROJECT_NAME} ${SRC_FILES})

add_subdirectory (Core)
add_subdirectory (MenuBase)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

include_directories(${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Core MenuBase)