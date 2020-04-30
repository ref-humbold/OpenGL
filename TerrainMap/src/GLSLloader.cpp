#include "GLSLloader.hpp"

#define READ_FROM_HEADER true

#if READ_FROM_HEADER
#include "shaders/AreaVertexShader_glsl.hpp"
#include "shaders/EarthVertexShader_glsl.hpp"
#include "shaders/FragmentShader_glsl.hpp"
#endif

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

std::string readShader(const std::string & filePath)
{
    // Read the shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(filePath, std::ios::in);

    std::cerr << ".::. Reading shader : " << filePath << "\n";

    if(!shaderStream.is_open())
        throw std::runtime_error("Impossible to open "s + filePath);

    std::string line = "";

    while(getline(shaderStream, line))
        shaderCode += "\n" + line;

    shaderStream.close();
    return shaderCode;
}

std::tuple<GLuint, GLuint> loadShaders()
{
    // Read shaders
#if READ_FROM_HEADER
    std::string areaVertexShaderCode =
            std::string(AreaVertexShader_glsl, AreaVertexShader_glsl_len);
    std::string earthVertexShaderCode =
            std::string(EarthVertexShader_glsl, EarthVertexShader_glsl_len);
    std::string fragmentShaderCode = std::string(FragmentShader_glsl, FragmentShader_glsl_len);
#else
    std::string areaVertexShaderCode = readShader("../shaders/AreaVertexShader.glsl"s);
    std::string earthVertexShaderCode = readShader("../shaders/EarthVertexShader.glsl"s);
    std::string fragmentShaderCode = readShader("../shaders/FragmentShader.glsl"s);
#endif

    // Create and compile shaders
    GLuint areaVertexShaderID =
            compileShader(GL_VERTEX_SHADER, areaVertexShaderCode, "AreaVertexShader.glsl"s);
    GLuint earthVertexShaderID =
            compileShader(GL_VERTEX_SHADER, earthVertexShaderCode, "EarthVertexShader.glsl"s);
    GLuint fragmentShaderID =
            compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode, "FragmentShader.glsl"s);

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
