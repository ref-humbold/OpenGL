#ifndef _GLSL_LOADER_HPP_
#define _GLSL_LOADER_HPP_

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

using namespace glm;

void compileShader(GLuint shader_ID, const std::string & shader_code);

GLuint linkProgram(GLuint vertex_shader_ID, GLuint fragment_shader_ID);

GLuint prepareShader(const std::string & file_path, GLenum shader_type);

GLuint loadShaders(const std::string & vertex_file_path, const std::string & fragment_file_path);

#endif
