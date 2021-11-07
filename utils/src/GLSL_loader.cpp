#include "GLSL_loader.hpp"
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std::string_literals;

std::vector<GLuint> GLSL_ShaderLoader::loadShaders()
{
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    // Read shaders
    switch(source)
    {
        case ShaderSource::Header:
            vertexShaderCode = VertexShader_glsl;
            fragmentShaderCode = FragmentShader_glsl;
            break;

        case ShaderSource::File:
            vertexShaderCode = readShader("../shaders/VertexShader.glsl"s);
            fragmentShaderCode = readShader("../shaders/FragmentShader.glsl"s);
            break;
    }

    GLSL_Shader vertexShader =
            GLSL_Shader(GL_VERTEX_SHADER, "VertexShader.glsl"s, vertexShaderCode);
    GLSL_Shader fragmentShader =
            GLSL_Shader(GL_FRAGMENT_SHADER, "FragmentShader.glsl"s, fragmentShaderCode);
    GLSL_CompiledShader compiledVertexShader = compileShader(vertexShader);
    GLSL_CompiledShader compiledFragmentShader = compileShader(fragmentShader);
    GLuint programID = linkProgram(compiledVertexShader, compiledFragmentShader);

    glDetachShader(programID, compiledVertexShader.id);
    glDetachShader(programID, compiledFragmentShader.id);

    glDeleteShader(compiledVertexShader.id);
    glDeleteShader(compiledFragmentShader.id);

    std::cerr << "[I] Shaders loaded!\n";

    return {programID};
}

GLSL_CompiledShader GLSL_ShaderLoader::compileShader(const GLSL_Shader & shader)
{
    std::cerr << "[I] Compiling shader : " << shader.name << "\n";

    GLint result = GL_FALSE;
    const char * shaderCodePointer = shader.code.c_str();
    GLuint shaderID = glCreateShader(shader.type);

    glShaderSource(shaderID, 1, &shaderCodePointer, nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        GLint infoLogLength = 0;

        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        if(infoLogLength > 0)
        {
            GLchar shaderErrorMessage[infoLogLength + 1];

            glGetShaderInfoLog(shaderID, infoLogLength, nullptr, shaderErrorMessage);
            throw ShaderError(shaderErrorMessage);
        }
        else
            throw ShaderError("Shader compilation failed: "s + shader.name);
    }

    return GLSL_CompiledShader(shader, shaderID);
}

GLuint GLSL_ShaderLoader::linkProgram(const GLSL_CompiledShader & vertexShader,
                                      const GLSL_CompiledShader & fragmentShader)
{
    std::cerr << "[I] Linking program\n";

    GLuint programID = glCreateProgram();
    GLint result = GL_FALSE;

    glAttachShader(programID, vertexShader.id);
    glAttachShader(programID, fragmentShader.id);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &result);

    if(result == GL_FALSE)
    {
        GLint infoLogLength = 0;

        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

        if(infoLogLength > 0)
        {
            GLchar programErrorMessage[infoLogLength + 1];

            glGetProgramInfoLog(programID, infoLogLength, nullptr, programErrorMessage);
            throw ShaderError(programErrorMessage);
        }
        else
            throw ShaderError("Linking program failed: "s + vertexShader.name + ", "
                              + fragmentShader.name);
    }

    return programID;
}

std::string GLSL_ShaderLoader::readShader(const std::string & filePath)
{
    std::string shaderCode;
    std::ifstream shaderStream(filePath, std::ios::in);

    std::cerr << "[I] Reading shader : " << filePath << "\n";

    if(!shaderStream.is_open())
        throw ShaderError("Impossible to open file "s + filePath);

    std::string line = "";

    while(getline(shaderStream, line))
        shaderCode += "\n" + line;

    shaderStream.close();
    return shaderCode;
}
