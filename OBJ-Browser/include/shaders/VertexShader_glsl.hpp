#ifndef VERTEXSHADER_GLSL_HPP_
#define VERTEXSHADER_GLSL_HPP_

#include <string>

const std::string VertexShader_glsl = R"VS(
#version 330 core

uniform mat4 worldToCameraMat;
uniform mat4 cameraToClipMat;

layout(location=0) in vec3 vertexPos;
layout(location=1) in vec2 vertexUV;
layout(location=2) in vec3 vertexNormal;

out vec4 fragmentColor;
out vec4 normVecCamera;
out vec4 observeDirCamera;

void main()
{
    mat4 mvMat = worldToCameraMat;
    mat4 mvpMat = cameraToClipMat * worldToCameraMat;

    vec4 vPosCamera = mvMat * vec4(vertexPos, 1.0);
    vec4 vPosClip = mvpMat * vec4(vertexPos, 1.0);

    fragmentColor = vec4(0.0, 0.0, 1.0, 1.0);
    normVecCamera = transpose(inverse(mvMat)) * vec4(vertexNormal, 0.0);
    observeDirCamera = -vPosCamera;

    gl_Position = vPosClip;
}
)VS";

#endif
