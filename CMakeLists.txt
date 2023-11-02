cmake_minimum_required (VERSION 2.6)

project (AngryBirds)

include_directories("src")

# Set the name of the executable to be produced
set(EXECUTABLE_NAME AngryBirds)

# Add all .cpp - files under src/ directory
file(GLOB SOURCES src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
set(CMAKE_BUILD_TYPE Debug)

# Find Box2D library
add_subdirectory(libs/box2d-2.4.1)
target_link_libraries(${PROJECT_NAME} box2d)

# Set SFML_DIR for windows
if(WIN32)
    set(SFML_DIR "libs/SFML-2.6.0/lib/cmake/SFML")
endif()

# Find any version 2.X of SFML
find_package(SFML 2 REQUIRED audio graphics window system)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})

    if(WIN32)
        target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
    endif()
    if(UNIX)
        target_link_libraries(${EXECUTABLE_NAME} sfml-window sfml-graphics sfml-audio sfml-system)
    endif()
endif()


# For installing targets (not mandatory)
install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
