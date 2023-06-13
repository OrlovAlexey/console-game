# Console game
Console C++ strategy game "Country Battle".

### Concept: Tower Defense
There are five lines, where units can go. Your side is on the left, right side is opponent's. Players spawn units and see them combating. When a unit reaches opponent's side, he hits it. The goal is to destroy your opponent's side (country).

#### Installing library ncurses
*Linux:* `sudo apt-get install libncursesw5-dev` \
*Mac:* `brew install ncurses` (you need to have homebrew installed)

#### Launch 
From dev directory run this command: `cmake CMakeLists.txt && make`

#### Code Description
Inheritance_map.html contains the picture of inheritance dependencies of units and countries.
