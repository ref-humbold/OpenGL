#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "GraphicObject.hpp"

using namespace glm;

void readOBJ(GraphicObject * grobj, const char * filename);

#endif
