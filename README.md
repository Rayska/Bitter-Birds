# Bitter Birds

## Introduction
An Angry Birds-like game with additional features based on C++, Box2D, and SFML.

This project was developed as a group project of Aalto University ELEC-A7151 Object-oriented Programming with C++ course in 2023. The responsible teacher for the course was Yusein Ali, and the project advisor was Tuan Nguyen Cong.

## Developed by
- Luukas Karihtala
- Ray Sointula
- Onni Komulainen
- Daniel Granström

## Project Implementation
Originally this project was developed on GitLab, and it was imported to GitHub after completion. Libraries used include SMFL, Box2D, and tools used include CMake, Make, Doxygen, and GCC.

## Documentation
The source code documentation can be found in the doc folder as [software_documentation.pdf](https://github.com/Rayska/Bitter-Birds/blob/master/doc/software_documentation.pdf).
This pdf was created using Doxygen.

Project documentation can also be found in the doc folder as [report.pdf](https://github.com/Rayska/Bitter-Birds/blob/master/doc/report.pdf).

## User's Manual

 ### How to use
After running the executable (instructions further down) you will be greeted by a request to enter your name.
After clicking “OK”, a menu screen will be shown. This menu allows you to start
playing the game, edit and make your own levels, or exit the game.

#### Playing:
The game is played by clicking and dragging on the slingshot on the left side of your
screen. This will shoot a bird in the opposite direction of your drag. The objective of
the game is to destroy all the enemies present in the level with as few of your birds
as possible. Some birds (Yellow and Blue) have special abilities, which can be
activated after launching by right-clicking. Yellow bird gains a speed boost and Blue
bird explodes. Keep in mind that you cannot activate a special ability of a bird if you
have already launched another bird, i.e. only the ability of the most recently launched
bird can be activated.



Indicators in the top left corner will display the number of birds you have left, and
which bird you will be launching next. In the top right corner of your screen you can
see your score. This score can be increased by destroying enemies and structures.
The more birds you use, the less score you will get for destroying enemies and
structures.

You can use the AD keys to move the camera when needed. You can exit into the
main menu by pressing esc whenever you like.

#### Editing levels:
Pre-existing levels can be edited by selecting them from the level selector, or a new
one can be created. Structures and enemies can be deleted by right-clicking them,
new ones can be created by selecting the corresponding button from the top of your
screen. Structures and enemies can be moved by left-clicking and dragging. The
slingshot can not be moved, deleted and new ones can not be created.

The number of birds and the type of these birds can be altered using the “Make
normal”, “Make yellow”, “Make blue”, and “Reset Birds” buttons.


### How to build the game
These instructions are applicable to only Linux/WSL.
1. Install cmake
```
sudo apt-get install cmake
```

2. Install make
```
sudo apt-get install make
```

3. Install libraries
```
sudo apt-get install libsfml-dev
```

4. Generate makefile with cmake
```
cmake .
```

5. Build project using cmake
```
make BitterBirds
```
 6. Run executable
 ```
 ./BitterBirds
 ```
