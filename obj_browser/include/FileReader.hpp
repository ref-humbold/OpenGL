#ifndef _FILE_READER_HPP_
#define _FILE_READER_HPP_

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "GraphicObject.hpp"

using namespace glm;

vec3 normalVec(vec3 vt1, vec3 vt2, vec3 vt3);

std::vector<std::string> split(const std::string & str, const std::string & delim);

std::vector<GLuint> parse(const std::string & str, const std::string & delim);

void readOBJ(GraphicObject * grobj, const char * filename);

#endif
