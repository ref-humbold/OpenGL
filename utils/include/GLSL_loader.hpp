#ifndef GLSL_LOADER_HPP_
#define GLSL_LOADER_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include "GLSL_shaders.hpp"

struct ShaderError : std::runtime_error
{
    explicit ShaderError(const std::string & s) : std::runtime_error(s)
    {
    }

    explicit ShaderError(const char * s) : std::runtime_error(s)
    {
    }
};

struct GLSL_Shader
{
    GLSL_Shader(GLenum type, const std::string & name, const std::string & code)
        : type{type}, name{name}, code{code}
    {
    }

    virtual ~GLSL_Shader() = default;

    GLenum type;
    std::string name;
    std::string code;
};

struct GLSL_CompiledShader : public Shader
{
    GLSL_CompiledShader(const GLSL_Shader & shader, GLuint id)
        : GLSL_Shader(shader.type, shader.name, shader.code), id{id}
    {
    }

    ~GLSL_CompiledShader() override = default;

    GLuint id;
};

enum class ShaderSource
{
    Header,
    File
};

class GLSL_ShaderLoader
{
public:
    explicit GLSL_ShaderLoader(ShaderSource source) : source{source}
    {
    }

    std::vector<GLuint> loadShaders();

private:
    std::string readShader(const std::string & filePath);
    GLSL_CompiledShader compileShader(const GLSL_Shader & shader);
    GLuint linkProgram(const GLSL_CompiledShader & vertexShader,
                       const GLSL_CompiledShader & fragmentShader);

    ShaderSource source;
};

#endif
