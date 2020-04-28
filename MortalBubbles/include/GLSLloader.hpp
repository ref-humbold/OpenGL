#ifndefGLSL_LOADER_HPP_
#defineGLSL_LOADER_HPP_

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

void compileShader(GLuint shaderID, const std::string & shaderCode);

GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

GLuint prepareShader(const std::string & filePath, GLenum shaderType);

GLuint loadShaders(const std::string & vertexFilePath, const std::string & fragmentFilePath);

#endif
