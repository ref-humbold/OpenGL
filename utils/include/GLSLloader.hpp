#ifndef GLSL_LOADER_HPP_
#define GLSL_LOADER_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>

struct shader_error : std::runtime_error
{
    explicit shader_error(const std::string & s) : std::runtime_error(s)
    {
    }
};

class ShaderLoader
{
public:
    ShaderLoader()
    {
    }

    ~ShaderLoader() = default;

    std::vector<GLuint> loadShaders();

private:
    GLuint compileShader(GLenum shaderType, const std::string & shaderCode,
                         const std::string & shaderName);
    GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
    std::string readShader(const std::string & filePath);
};

#endif
