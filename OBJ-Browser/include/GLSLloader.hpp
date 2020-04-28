#ifndef GLSL_LOADER_HPP_
#define GLSL_LOADER_HPP_

#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

GLuint compileShader(GLenum shaderType, const std::string & shaderCode,
                     const std::string & shaderName);

GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

GLuint loadShaders();

#endif
