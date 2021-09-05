#ifndef GLSL_LOADER_HPP_
#define GLSL_LOADER_HPP_

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

GLuint compileShader(GLenum shaderType, const std::string & shaderCode,
                     const std::string & shaderName);

GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

std::string readShader(const std::string & filePath);

GLuint loadShaders();

#endif
