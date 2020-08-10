#ifndef AREAVERTEXSHADER_GLSL_HPP_
#define AREAVERTEXSHADER_GLSL_HPP_

#include <string>

const std::string AreaVertexShader_glsl = R"AVS(
#version 330 core

uniform mat4 worldToCameraMat;
uniform float longitude;
uniform float latitude;
uniform float mercator;
uniform int dimensions;

layout(location=0) in vec2 vertexPos;
layout(location=1) in float vertexHeight;

out vec4 fragmentColor;

const float PI = 3.1415926535897932384626433832795028841971693993751;
const float RADIUS = 6400000.0;

void main()
{
    float mapPosX = 0.0;
    float mapPosY = 0.0;
    float mapPosZ = 0.0;
    float lng = radians(longitude + vertexPos.x);
    float lat = radians(latitude + vertexPos.y);

    if(dimensions == 2)
    {
        mapPosX = mercator * lng;
        mapPosY = mercator * log(tan(PI / 4 + lat / 2));
    }
    else if(dimensions == 3)
    {
        mapPosX = (RADIUS + vertexHeight) * cos(lat) * sin(lng);
        mapPosY = (RADIUS + vertexHeight) * sin(lat);
        mapPosZ = (RADIUS + vertexHeight) * cos(lat) * cos(lng);
    }

    if(vertexHeight < -32760)
    {
        fragmentColor = vec4(0.5, 0.5, 0.5, 1.0);
        mapPosX = RADIUS * cos(lat) * sin(lng);
        mapPosY = RADIUS * sin(lat);
        mapPosZ = RADIUS * cos(lat) * cos(lng);
    }
    else if(vertexHeight == 0)
    {
        fragmentColor = vec4(0.0,
                             0.0,
                             1.0,
                             1.0);
    }
    else if(vertexHeight <= 500)
    {
        fragmentColor = vec4(0.0,
                             vertexHeight / 500.0,
                             0.0,
                             1.0);
    }
    else if(vertexHeight <= 1000)
    {
        fragmentColor = vec4(vertexHeight / 500.0 - 1.0,
                             1.0,
                             0.0,
                             1.0);
    }
    else if(vertexHeight <= 2000)
    {
        fragmentColor = vec4(1.0,
                             2.0 - vertexHeight / 1000.0,
                             0.0,
                             1.0);
    }
    else if(vertexHeight <= 3000)
    {
        fragmentColor = vec4(1.4 - vertexHeight / 5000.0,
                             vertexHeight / 2000.0 - 1.0,
                             vertexHeight / 5000.0 - 0.4,
                             1.0);
    }
    else if(vertexHeight <= 4000)
    {
        fragmentColor = vec4(0.2 + vertexHeight / 5000.0,
                             vertexHeight / 2000.0 - 1.0,
                             vertexHeight / 1250.0 - 2.2,
                             1.0);
    }
    else
    {
        fragmentColor = vec4(1.0,
                             1.0,
                             1.0,
                             1.0);
    }

    vec4 vPos = vec4(mapPosX, mapPosY, mapPosZ, 1.0);
    vec4 vPosOut = worldToCameraMat * vPos;

    gl_Position = vPosOut;
}
)AVS";

#endif
