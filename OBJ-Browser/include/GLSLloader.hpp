#ifndef GLSL_LOADER_HPP_
#define GLSL_LOADER_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <GL/glew.h>

struct shader_error : std::runtime_error
{
    explicit shader_error(const std::string & s) : std::runtime_error(s)
    {
    }
};

GLuint loadShaders();

#endif
