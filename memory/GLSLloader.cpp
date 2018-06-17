#include "GLSLloader.hpp"

using namespace glm;

void compileShader(GLuint ShaderID, std::string ShaderCode)
{
    GLint result = GL_FALSE;
    int InfoLogLength;

    // Compile shader
    char const * SourcePointer = ShaderCode.c_str();

    glShaderSource(ShaderID, 1, &SourcePointer, nullptr);
    glCompileShader(ShaderID);

    // Check shader
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0)
    {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);

        glGetShaderInfoLog(ShaderID, InfoLogLength, nullptr, &ShaderErrorMessage[0]);
        throw std::runtime_error(&ShaderErrorMessage[0]);
    }
}

GLuint linkProgram(GLuint VertexShaderID, GLuint FragmentShaderID)
{
    GLint result = GL_FALSE;
    int InfoLogLength;

    // Link the program
    GLuint ProgramID = glCreateProgram();

    std::cout << "Linking program\n";
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);

        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        std::cout << &ProgramErrorMessage[0] << "\n";
    }

    return ProgramID;
}

GLuint prepareShader(std::string file_path, GLenum shader_type)
{
    // Read the shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file_path, std::ios::in);

    if(shaderStream.is_open())
    {
        std::string Line = "";

        while(getline(shaderStream, Line))
            shaderCode += "\n" + Line;

        shaderStream.close();
    }
    else
        throw std::runtime_error(std::string("Impossible to open ") + file_path);

    GLuint shaderID = glCreateShader(shader_type);

    std::cout << "Compiling shader : " << file_path << "\n";
    compileShader(shaderID, shaderCode);

    return shaderID;
}

GLuint loadShaders(std::string vertex_file_path, std::string fragment_file_path)
{
    // Create and compile shaders
    GLuint VertexShaderID = prepareShader(vertex_file_path, GL_VERTEX_SHADER);
    GLuint FragmentShaderID = prepareShader(fragment_file_path, GL_FRAGMENT_SHADER);

    GLuint ProgramID = linkProgram(VertexShaderID, FragmentShaderID);

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
