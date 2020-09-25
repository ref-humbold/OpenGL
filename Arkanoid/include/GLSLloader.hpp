#ifndef GLSL_LOADER_HPP_
#define GLSL_LOADER_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <GL/glew.h>

struct shader_exception : std::runtime_error
{
    explicit shader_exception(const std::string & s) : std::runtime_error(s)
    {
    }
};

GLuint compileShader(GLenum shaderType, const std::string & shaderCode,
                     const std::string & shaderName);

GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

std::string readShader(const std::string & filePath);

GLuint loadShaders();

#endif
