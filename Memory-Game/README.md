# Memory-Game
Simple memory game in C++ & OpenGL

## About
Memory game is a small single-player game written in C++ language with OpenGL. Your goal is to match all the pairs of cards in the smallest number of moves.

### Keys in use
*up*, *down*, *left*, *right* : choose next card to open \
*space* : open the chosen card; start new game when finished
*esc* : close the game

-----

## Dependencies

### Standard build & run
> *versions last used by the author are in double parentheses and italic*

General:
+ Linux-based operating system \
  *((Debian testing))*
+ C++ compiler \
  *((APT package `g++`, 10.3.+))*
+ [CMake](https://cmake.org/) \
  *((APT package `cmake`, 3.18.+))*
+ [GNU Make](https://www.gnu.org/software/make) \
  *((APT package `make`, 4.3.+))*
+ OpenGL, GLFW, GLEW, GLM

### Automated formatting
+ [Clang-format](https://releases.llvm.org/11.0.0/tools/clang/docs/ClangFormat.html) \
  *((APT package `clang-format`, 11.+))*

-----

## How to build?
Game can be built using **CMake** that generates **GNU Make** Makefile.

Follow these steps to build:
```sh
$ cd /path-to-project-directory/
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## How to run?
Game can be run directly using the executable file in the `bin` root directory:
```sh
$ /path-to-project-directory/bin/MemoryGame [-r rows] [-c columns]
```

**`-r`** `rows` : number of rows, must be between 1 and 12 (default 4) \
**`-c`** `columns` : number of columns, must be between 1 and 12 (default 4)
