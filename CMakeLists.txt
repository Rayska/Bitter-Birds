cmake_minimum_required(VERSION 3.5)

project(BitterBirds)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("src")
include_directories("tests")

# Set the name of the executable to be produced
set(EXECUTABLE_NAME BitterBirds)

# Add all .cpp - files under src/ directory
file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
set(CMAKE_BUILD_TYPE Debug)

# Find Box2D library
add_subdirectory(libs/box2d-2.4.1)
target_link_libraries(${PROJECT_NAME} box2d)

# Find any version 2.X of SFML
find_package(SFML 2 REQUIRED audio graphics window system)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} sfml-window sfml-graphics sfml-audio sfml-system)
endif()

# For installing targets (not mandatory)
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# Unit tests
file(GLOB SOURCES2 src/*.cpp)
list(REMOVE_ITEM SOURCES2 ${CMAKE_SOURCE_DIR}/src/main.cpp)
file(GLOB SOURCES3 tests/*.cpp)

add_executable(unittests ${SOURCES2} ${SOURCES3})

# Link libraries for unit tests
target_link_libraries(unittests sfml-window sfml-graphics sfml-audio sfml-system)
target_link_libraries(unittests box2d)
