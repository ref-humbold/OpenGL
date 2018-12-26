#include "GLSLloader.hpp"

using namespace glm;
using namespace std::string_literals;

void compileShader(GLuint shaderID, const std::string & shaderCode)
{
    GLint result = GL_FALSE;
    int infoLogLength;

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

GLuint prepareShader(const std::string & filePath, GLenum shaderType)
{
    // Read the shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(filePath, std::ios::in);

    if(shaderStream.is_open())
        throw std::runtime_error("Impossible to open "s + filePath);

    std::string line = "";

    while(getline(shaderStream, line))
        shaderCode += "\n" + line;

    shaderStream.close();

    GLuint shaderID = glCreateShader(shaderType);

    std::cerr << ".::. Compiling shader : " << filePath << "\n";
    compileShader(shaderID, shaderCode);

    return shaderID;
}

std::tuple<GLuint, GLuint> loadShaders(const std::string & areaVertexFilePath,
                                       const std::string & earthVertexFilePath,
                                       const std::string & fragmentFilePath)
{
    // Create and compile shaders
    GLuint areaVertexShaderID = prepareShader(areaVertexFilePath, GL_VERTEX_SHADER);
    GLuint earthVertexShaderID = prepareShader(earthVertexFilePath, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = prepareShader(fragmentFilePath, GL_FRAGMENT_SHADER);

    GLuint areaProgramID = linkProgram(areaVertexShaderID, fragmentShaderID);
    GLuint earthProgramID = linkProgram(earthVertexShaderID, fragmentShaderID);

    glDetachShader(areaProgramID, areaVertexShaderID);
    glDetachShader(areaProgramID, fragmentShaderID);
    glDetachShader(earthProgramID, earthVertexShaderID);
    glDetachShader(earthProgramID, fragmentShaderID);

    glDeleteShader(areaVertexShaderID);
    glDeleteShader(earthVertexShaderID);
    glDeleteShader(fragmentShaderID);

    std::cerr << ".::. Shaders loaded!\n";

    return std::make_tuple(areaProgramID, earthProgramID);
}
