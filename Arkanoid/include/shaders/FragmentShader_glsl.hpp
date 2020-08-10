#ifndef FRAGMENTSHADER_GLSL_HPP_
#define FRAGMENTSHADER_GLSL_HPP_

#include <string>

const std::string FragmentShader_glsl = R"FS(
#version 330 core

in vec3 fragmentColor;

out vec3 outColor;

void main()
{
    outColor = fragmentColor;
}
)FS";

#endif
