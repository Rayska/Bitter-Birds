
cmake_minimum_required (VERSION 2.6)
project (MyGame)
include_directories("src")

# Set the name of the executable to be produced
set(EXECUTABLE_NAME MyGame)

# Add all .cpp - files under src/ directory
file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
set(CMAKE_BUILD_TYPE Debug)

# Find Box2D library
add_subdirectory(libs/windows/box2d-2.4.1)
target_link_libraries(${PROJECT_NAME} box2d)

# Set SFML_DIR
set(SFML_DIR "libs/windows/SFML-2.6.0/lib/cmake/SFML")

# Find any version 2.X of SFML
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# For installing targets (not mandatory)
install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
