#include "GLSLloader.hpp"
#include "shaders/FragmentShader_glsl.hpp"
#include "shaders/VertexShader_glsl.hpp"

using namespace glm;
using namespace std::string_literals;

GLuint compileShader(GLenum shaderType, const std::string & shaderCode,
                     const std::string & shaderName)
{
    GLuint shaderID = glCreateShader(shaderType);
    GLint result = GL_FALSE;
    int infoLogLength;

    std::cerr << ".::. Compiling shader : " << shaderName << "\n";

    // Compile shader
    const char * shaderCodePointer = shaderCode.c_str();

    glShaderSource(shaderID, 1, &shaderCodePointer, nullptr);
    glCompileShader(shaderID);

    // Check shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0)
    {
        std::string shaderErrorMessage(infoLogLength + 1, '\0');

        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &shaderErrorMessage[0]);
        throw std::runtime_error(shaderErrorMessage);
    }

    return shaderID;
}

GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
    GLint result = GL_FALSE;
    int infoLogLength;

    // Link the program
    GLuint programID = glCreateProgram();

    std::cerr << ".::. Linking program\n";
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0)
    {
        std::string programErrorMessage(infoLogLength + 1, '\0');

        glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
        throw std::runtime_error(programErrorMessage);
    }

    return programID;
}

GLuint loadShaders()
{
    // Create and compile shaders
    GLuint vertexShaderID =
            compileShader(GL_VERTEX_SHADER, std::string(VertexShader_glsl, VertexShader_glsl_len),
                          "VertexShader.glsl"s);
    GLuint fragmentShaderID = compileShader(
            GL_FRAGMENT_SHADER, std::string(FragmentShader_glsl, FragmentShader_glsl_len),
            "FragmentShader.glsl"s);

    GLuint programID = linkProgram(vertexShaderID, fragmentShaderID);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    std::cerr << ".::. Shaders loaded!\n";

    return programID;
}
