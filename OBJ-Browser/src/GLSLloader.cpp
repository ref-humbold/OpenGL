#include "GLSLloader.hpp"

#define READ_FROM_HEADER true

#if READ_FROM_HEADER
#include "shaders/FragmentShader_glsl.hpp"
#include "shaders/VertexShader_glsl.hpp"
#endif

using namespace glm;
using namespace std::string_literals;

GLuint compileShader(GLenum shaderType, const std::string & shaderCode,
                     const std::string & shaderName)
{
    GLuint shaderID = glCreateShader(shaderType);
    GLint result = GL_FALSE;
    int infoLogLength;

    std::cerr << "[I] Compiling shader : " << shaderName << "\n";

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

    std::cerr << "[I] Linking program\n";
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

std::string readShader(const std::string & filePath)
{
    // Read the shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(filePath, std::ios::in);

    std::cerr << "[I] Reading shader : " << filePath << "\n";

    if(!shaderStream.is_open())
        throw std::runtime_error("Impossible to open "s + filePath);

    std::string line = "";

    while(getline(shaderStream, line))
        shaderCode += "\n" + line;

    shaderStream.close();
    return shaderCode;
}

GLuint loadShaders()
{
    // Read shaders
#if READ_FROM_HEADER
    const std::string & vertexShaderCode = VertexShader_glsl;
    const std::string & fragmentShaderCode = FragmentShader_glsl;
#else
    std::string vertexShaderCode = readShader("../shaders/VertexShader.glsl"s);
    std::string fragmentShaderCode = readShader("../shaders/FragmentShader.glsl"s);
#endif

    // Create and compile shaders
    GLuint vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderCode, "VertexShader.glsl"s);
    GLuint fragmentShaderID =
            compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode, "FragmentShader.glsl"s);

    GLuint programID = linkProgram(vertexShaderID, fragmentShaderID);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    std::cerr << "[I] Shaders loaded!\n";

    return programID;
}
