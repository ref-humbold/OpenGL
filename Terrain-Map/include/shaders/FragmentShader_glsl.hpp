#ifndef FRAGMENTSHADER_GLSL_HPP_
#define FRAGMENTSHADER_GLSL_HPP_

#include <string>

const std::string FragmentShader_glsl = R"FS(
#version 330 core

in vec4 fragmentColor;
out vec4 outColor;

void main()
{
    outColor = fragmentColor;
}
)FS";

#endif
