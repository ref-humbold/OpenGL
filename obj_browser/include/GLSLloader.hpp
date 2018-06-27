#ifndef _GLSL_LOADER_HPP_
#define _GLSL_LOADER_HPP_

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

void compileShader(GLuint ShaderID, std::string ShaderCode);

GLuint linkProgram(GLuint VertexShaderID, GLuint FragmentShaderID);

GLuint prepareShader(std::string file_path, GLenum shader_type);

GLuint loadShaders(std::string vertex_file_path, std::string fragment_file_path);

#endif
